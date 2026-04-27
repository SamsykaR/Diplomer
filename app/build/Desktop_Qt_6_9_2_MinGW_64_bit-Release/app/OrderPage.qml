import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    title: "Оформление заказа"

    property var fullMenuModel: []
    ListModel { id: filteredMenuModel }
    ListModel { id: orderModel }
    property int selectedMenuIndex: -1

    function reloadData() {
        loger.info("OrderPage: reloading foods")
        loadFoods()
    }

    Connections {
        target: window
        function onDataRefreshRequested() {
            reloadData()
        }
    }

    function loadFoods() {
        var foods = dbWorker.getFoods()
        fullMenuModel = foods
        applyFilters()
    }

    Component.onCompleted: loadFoods()

    function applyFilters() {
        filteredMenuModel.clear()
        for (var i = 0; i < fullMenuModel.length; i++) {
            var item = fullMenuModel[i]
            if (categoryFilter.currentIndex > 0 && item.category !== categoryFilter.model[categoryFilter.currentIndex])
                continue
            if (availableOnly.checked && !item.available)
                continue
            filteredMenuModel.append(item)
        }
        if (selectedMenuIndex >= filteredMenuModel.count)
            selectedMenuIndex = -1
    }

    function addDishToOrder(foodId, quantity, dish) {
        var success = dbWorker.checkAndConsume(foodId, quantity)
        if (success) {
            var found = false
            for (var i = 0; i < orderModel.count; i++) {
                if (orderModel.get(i).foodId === foodId) {
                    var newQty = orderModel.get(i).quantity + quantity
                    orderModel.setProperty(i, "quantity", newQty)
                    found = true
                    break
                }
            }
            if (!found) {
                orderModel.append({
                    name: dish.name,
                    foodId: dish.foodId,
                    price: dish.price,
                    quantity: quantity
                })
            }
        } else {
            errorDialog.message = "Недостаточно продуктов для добавления блюда"
            errorDialog.open()
        }
    }

    Dialog {
        id: warningDialog
        title: "Предупреждение"
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        width: 500
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        property var dishInfo: ({})
        property string message: ""
        onAccepted: {
            addDishToOrder(dishInfo.foodId, dishInfo.quantity, dishInfo.dish)
        }
        contentItem: Label {
            text: warningDialog.message
            wrapMode: Text.WordWrap
            font.pixelSize: 24
            width: warningDialog.width - 40
        }
    }

    Dialog {
        id: errorDialog
        title: "Ошибка"
        modal: true
        standardButtons: Dialog.Ok
        width: 500
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        property string message: ""
        contentItem: Label {
            text: errorDialog.message
            wrapMode: Text.WordWrap
            font.pixelSize: 24
            width: errorDialog.width - 40
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Левая колонка – меню с фильтрами
        ColumnLayout {
            Layout.preferredWidth: parent.width * 0.5
            Layout.fillHeight: true
            spacing: 10

            RowLayout {
                Layout.fillWidth: true
                ComboBox {
                    id: categoryFilter
                    Layout.fillWidth: true
                    model: ["Все категории", "Супы", "Горячее", "Салаты", "Напитки"]
                    onCurrentIndexChanged: applyFilters()
                }
                CheckBox {
                    id: availableOnly
                    text: "Только доступные"
                    onCheckedChanged: applyFilters()
                }
            }

            ListView {
                id: menuListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: filteredMenuModel
                delegate: Item {
                    width: menuListView.width
                    height: contentColumn.implicitHeight + 20

                    Rectangle {
                        id: card
                        anchors.fill: parent
                        anchors.margins: 5
                        radius: 8
                        color: menuListView.currentIndex === index ? "#E8F5E9" : "#FFFFFF"
                        border.color: "#DADADA"
                        border.width: 1
                    }

                    ColumnLayout {
                        id: contentColumn
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 6

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                text: model.name
                                font.pixelSize: 18
                                font.bold: true
                            }
                            Label {
                                text: model.price + " ₽"
                                font.pixelSize: 16
                                color: Material.accent
                                Layout.alignment: Qt.AlignRight
                            }
                        }

                        Label {
                            text: model.category
                            font.pixelSize: 14
                            color: "#666666"
                        }

                        Label {
                            text: model.description
                            font.pixelSize: 14
                            color: "#777777"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        Label {
                            text: "Состав: " + model.composition
                            font.pixelSize: 13
                            color: "#888888"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        Label {
                            visible: !model.available
                            text: "Нет в наличии"
                            font.pixelSize: 13
                            color: "#C62828"
                            font.bold: true
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            menuListView.currentIndex = index
                            selectedMenuIndex = index
                        }
                    }
                }
            }
        }

        // Правая колонка – текущий заказ
        ColumnLayout {
            Layout.preferredWidth: parent.width * 0.5
            Layout.fillHeight: true
            spacing: 10

            GroupBox {
                title: "Текущий заказ"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: orderListView
                    anchors.fill: parent
                    clip: true
                    model: orderModel
                    delegate: ItemDelegate {
                        width: parent ? parent.width : 0
                        height: 50
                        highlighted: ListView.isCurrentItem
                        text: model.name + " x" + model.quantity
                        onClicked: orderListView.currentIndex = index
                    }
                }
            }

            GridLayout {
                columns: 2
                columnSpacing: 10
                rowSpacing: 10
                Layout.fillWidth: true

                Label { text: "Столик №:" }
                SpinBox {
                    id: tableSpinBox
                    from: 1; to: 50; value: 1
                    editable: true
                    Layout.fillWidth: true
                }

                Label { text: "Кол-во:" }
                SpinBox {
                    id: quantitySpinBox
                    from: 1; to: 10; value: 1
                    Layout.fillWidth: true
                }

                Label { text: "Комментарий:" }
                TextField {
                    id: requestField
                    placeholderText: "без лука и т.п."
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    text: "Добавить"
                    Layout.fillWidth: true
                    enabled: selectedMenuIndex >= 0
                    onClicked: {
                        var dish = filteredMenuModel.get(selectedMenuIndex)
                        if (!dish.available) {
                            errorDialog.message = "Это блюдо временно недоступно"
                            errorDialog.open()
                            return
                        }

                        var warnings = dbWorker.getLowStockWarning(dish.foodId, quantitySpinBox.value)
                        if (warnings.length > 0) {
                            var msg = "После добавления следующие продукты будут ниже минимального порога:\n"
                            for (var i = 0; i < warnings.length; i++) {
                                msg += "• " + warnings[i].productName + ": "
                                msg += warnings[i].newQuantity + " (мин. " + warnings[i].minThreshold + ")\n"
                            }
                            msg += "\nПродолжить добавление?"
                            warningDialog.message = msg
                            warningDialog.dishInfo = { foodId: dish.foodId, quantity: quantitySpinBox.value, dish: dish }
                            warningDialog.open()
                        } else {
                            addDishToOrder(dish.foodId, quantitySpinBox.value, dish)
                        }
                    }
                }

                Button {
                    text: "Удалить"
                    Layout.fillWidth: true
                    enabled: orderListView.currentIndex >= 0
                    onClicked: {
                        var item = orderModel.get(orderListView.currentIndex)
                        if (dbWorker.returnIngredients(item.foodId, item.quantity)) {
                            orderModel.remove(orderListView.currentIndex)
                            orderListView.currentIndex = -1
                        } else {
                            errorDialog.message = "Не удалось вернуть продукты. Удаление отменено."
                            errorDialog.open()
                        }
                    }
                }
            }

            Button {
                text: "Оформить заказ"
                Layout.fillWidth: true
                height: 50
                font.pixelSize: 16
                highlighted: true
                Material.background: Material.Green
                onClicked: {
                    if (orderModel.count === 0) return

                    var total = 0
                    for (var i = 0; i < orderModel.count; i++) {
                        var item = orderModel.get(i)
                        total += item.price * item.quantity
                    }

                    var userId = dbWorker.getUserId()
                    var orderId = dbWorker.createOrder(tableSpinBox.value, userId, requestField.text, total)

                    if (orderId > 0) {
                        var success = true
                        for (var i = 0; i < orderModel.count; i++) {
                            var item = orderModel.get(i)
                            if (!dbWorker.addOrderItem(orderId, item.foodId, item.quantity, item.price)) {
                                success = false
                                break
                            }
                        }
                        if (success) {
                            orderModel.clear()
                            window.dataRefreshRequested()
                        } else {
                            loger.error("Ошибка добавления позиций заказа")
                        }
                    } else {
                        loger.error("Ошибка создания заказа")
                    }
                }
            }
        }
    }
}
