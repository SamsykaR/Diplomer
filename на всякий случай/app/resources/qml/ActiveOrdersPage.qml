import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    title: "Активные заказы"

    //! Текущий фильтр по статусу (пустая строка – все заказы).
    property string currentStatusFilter: ""
    //! Модель списка заказов.
    ListModel { id: ordersModel }
    //! Роль текущего пользователя.
    property string userRole: window.currentUser ? window.currentUser.role : ""

    /*!
     * \brief Загружает активные заказы за сегодняшний день с учётом фильтров.
     */
    function loadOrders() {
        var orders = dbWorker.getOrders(currentStatusFilter)
        var today = new Date()
        today.setHours(0, 0, 0, 0)
        orders = orders.filter(function(order) {
            var orderDate = new Date(order.orderDate)
            orderDate.setHours(0, 0, 0, 0)
            return orderDate.getTime() === today.getTime()
        })
        if (hideClosedCheckbox.checked) {
            orders = orders.filter(function(order) { return order.status !== "Закрыт" })
        }
        ordersModel.clear()
        for (var i = 0; i < orders.length; i++) ordersModel.append(orders[i])
        orderListView.model = ordersModel   // <-- ВАЖНО
    }

    Component.onCompleted: {
        loadOrders()
        dbWorker.ordersChanged.connect(loadOrders)
    }

    Dialog {
        id: checkDialog
        title: "Чек заказа №" + (checkDialog.orderNumber || "")
        modal: true
        width: 500
        height: 400
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        property int orderId: 0
        property int orderNumber: 0

        /*!
         * \brief Загружает данные чека по ID заказа и отображает их.
         * \param orderId ID заказа.
         * \param orderNumber Номер заказа.
         */
        function loadCheck(orderId, orderNumber) {
            checkDialog.orderId = orderId
            checkDialog.orderNumber = orderNumber
            var check = dbWorker.getCheckByOrderId(orderId)
            if (check && check.checkData) {
                var data = JSON.parse(check.checkData)
                var text = "Чек №: " + check.checkNumber + "\n"
                text += "Сумма: " + check.totalSum.toFixed(2) + " руб.\n"
                text += "Дата: " + new Date(check.createdAt).toLocaleString() + "\n\n"
                text += "Позиции:\n"
                for (var i = 0; i < data.length; i++) {
                    text += "• " + data[i].name + " x" + data[i].quantity + " = " + (data[i].price * data[i].quantity).toFixed(2) + " руб.\n"
                }
                checkText.text = text
            } else {
                checkText.text = "Чек не найден"
            }
        }

        contentItem: ScrollView {
            TextArea {
                id: checkText
                readOnly: true
                wrapMode: TextEdit.Wrap
                font.pixelSize: 14
            }
        }
        footer: DialogButtonBox {
            standardButtons: DialogButtonBox.Ok
            onAccepted: checkDialog.close()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Label { text: "Статус:" }
            ComboBox {
                id: statusFilter
                model: ["Все", "Активный", "Готовится", "Готов", "Закрыт"]
                currentIndex: 0
                onCurrentIndexChanged: {
                    if (currentIndex === 0) currentStatusFilter = ""
                    else currentStatusFilter = model[currentIndex]
                    loadOrders()
                }
            }
            CheckBox {
                id: hideClosedCheckbox
                text: "Скрыть закрытые"
                onCheckedChanged: loadOrders()
            }
        }

        ListView {
            id: orderListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10

            delegate: Rectangle {
                width: orderListView.width
                height: 140   // чуть выше, чтобы поместилась строка с официантом
                color: index % 2 === 0 ? "#F5F5F5" : "white"
                border.color: "#DDD"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Заказ №" + (model.orderNumber ? model.orderNumber : "?"); font.bold: true; font.pixelSize: 18 }
                        Label { text: "Столик: " + (model.tableNumber ? model.tableNumber : "?"); Layout.alignment: Qt.AlignRight }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Официант: " + (model.waiterName ? model.waiterName : "не указан") }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Дата: " + (model.orderDate ? new Date(model.orderDate).toLocaleString() : "") }
                        Label { text: "Сумма: " + (model.totalCost ? model.totalCost.toFixed(2) : "0") + " ₽"; Layout.alignment: Qt.AlignRight; font.bold: true; color: Material.accent }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Статус: " + (model.status === "Новый" ? "Активный" : model.status); font.italic: true }
                        Item { Layout.fillWidth: true }

                        Button {
                            text: "Изменить"
                            visible: model.status !== "Закрыт" && model.orderId > 0
                            onClicked: {
                                stackView.push("OrderPage.qml", { editMode: true, orderId: model.orderId })
                            }
                        }
                        Button {
                            text: "Готов"
                            visible: userRole === "admin" && model.status === "Готовится"
                            onClicked: {
                                if (dbWorker.updateOrderStatus(model.orderId, "Готов"))
                                    loadOrders()
                                else
                                    errorDialog.message = "Ошибка изменения статуса"
                            }
                        }
                        Button {
                            text: "Оплатить"
                            visible: (userRole === "waiter" || userRole === "admin") && model.status === "Готов"
                            onClicked: {
                                var items = dbWorker.getOrderItems(model.orderId)
                                if (items.length === 0) {
                                    errorDialog.message = "Нет позиций для оплаты"
                                    return
                                }
                                var itemsArray = []
                                for (var i = 0; i < items.length; i++) {
                                    itemsArray.push({
                                        name: items[i].name,
                                        quantity: items[i].quantity,
                                        price: items[i].price
                                    })
                                }
                                if (dbWorker.closeOrder(model.orderId, model.totalCost, itemsArray)) {
                                    loadOrders()
                                } else {
                                    errorDialog.message = "Ошибка при закрытии заказа"
                                }
                            }
                        }
                        Button {
                            text: "Просмотр чека"
                            visible: model.status === "Закрыт"
                            onClicked: {
                                checkDialog.loadCheck(model.orderId, model.orderNumber)
                                checkDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: errorDialog
        title: "Ошибка"
        modal: true
        standardButtons: Dialog.Ok
        property string message: ""
        contentItem: Label { text: errorDialog.message; wrapMode: Text.WordWrap }
    }
}
