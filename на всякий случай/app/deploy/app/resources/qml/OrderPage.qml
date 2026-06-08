import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    title: editMode ? "Редактирование заказа" : "Оформление заказа"

    //! \brief true, если страница открыта в режиме редактирования существующего заказа.
    property bool editMode: false
    //! \brief ID заказа для редактирования (актуально только при editMode == true).
    property int orderId: -1

    //! \brief Полное меню, загруженное с сервера.
    property var fullMenuModel: []
    //! \brief Отфильтрованное меню для отображения.
    ListModel { id: filteredMenuModel }
    //! \brief Корзина текущего заказа.
    ListModel { id: orderModel }
    //! \brief Индекс выбранного блюда в меню.
    property int selectedMenuIndex: -1

    /*!
     * \brief Загружает меню с сервера и применяет фильтры.
     */
    function loadFoods() {
        var foods = dbWorker.getFoods()
        fullMenuModel = foods
        applyFilters()
    }

    /*!
     * \brief Загружает данные заказа для редактирования.
     */
    function loadOrderForEdit() {
            if (!editMode || orderId <= 0) return

            var orders = dbWorker.getOrders("")
            for (var i = 0; i < orders.length; i++) {
                if (orders[i].orderId === orderId) {
                    tableSpinBox.value = orders[i].tableNumber
                    requestField.text = orders[i].specialRequests || ""
                    break
                }
            }
            var items = dbWorker.getOrderItems(orderId)
            orderModel.clear()
            for (var j = 0; j < items.length; j++) {
                orderModel.append({
                    name: items[j].name,
                    foodId: items[j].foodId,
                    price: items[j].price,
                    quantity: items[j].quantity
                })
            }
            loadFoods()
        }

    /*!
     * \brief Обновляет все данные страницы (синхронизация).
     */
    function reloadData() {
        loger.info("OrderPage: reloading data")
        if (editMode && orderId > 0)
            loadOrderForEdit()
        else
            loadFoods()
    }

    // При загрузке страницы
    Component.onCompleted: {
        if (editMode && orderId > 0)
            loadOrderForEdit()
        else
            loadFoods()
    }

    // Подписка на сигнал обновления из главного окна
    Connections {
        target: window
        function onDataRefreshRequested() {
            reloadData()
        }
    }

    // Фильтрация меню по категории и доступности
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

    // Добавление блюда в корзину (без списания в режиме редактирования!)
    function addDishToOrderInternal(foodId, quantity, dish) {
        if (!editMode) {
            // Обычный режим: сразу списываем ингредиенты
            var success = dbWorker.checkAndConsume(foodId, quantity)
            if (!success) {
                errorDialog.message = "Недостаточно продуктов для добавления блюда"
                errorDialog.open()
                return false
            }
        }
        // Добавляем в модель корзины
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
        return true
    }

    // Диалог предупреждения о малом остатке
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
            addDishToOrderInternal(dishInfo.foodId, dishInfo.quantity, dishInfo.dish)
        }
        contentItem: Label {
            text: warningDialog.message
            wrapMode: Text.WordWrap
            font.pixelSize: 24
            width: warningDialog.width - 40
        }
    }

    // Диалог ошибки
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

    // Основная компоновка
    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // ── Левая часть: меню ──
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
                            Label { text: model.name; font.pixelSize: 18; font.bold: true }
                            Label { text: model.price + " ₽"; font.pixelSize: 16; color: Material.accent; Layout.alignment: Qt.AlignRight }
                        }
                        Label { text: model.category; font.pixelSize: 14; color: "#666666" }
                        Label { text: model.description; font.pixelSize: 14; color: "#777777"; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                        Label { text: "Состав: " + model.composition; font.pixelSize: 13; color: "#888888"; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                        Label { visible: !model.available; text: "Нет в наличии"; font.pixelSize: 13; color: "#C62828"; font.bold: true }
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

        // ── Правая часть: заказ ──
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
                    enabled: !editMode
                    Layout.fillWidth: true
                }

                Label { text: "Кол-во:" }
                SpinBox {
                    id: quantitySpinBox
                    from: 1; to: 10; value: 1
                    editable: true
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

                        if (!editMode) {
                            // Обычный режим: проверяем предупреждение о малом остатке
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
                                return
                            }
                        }
                        // Добавляем блюдо (в режиме редактирования без списания)
                        addDishToOrderInternal(dish.foodId, quantitySpinBox.value, dish)
                    }
                }

                Button {
                    text: "Удалить"
                    Layout.fillWidth: true
                    enabled: orderListView.currentIndex >= 0
                    onClicked: {
                        var item = orderModel.get(orderListView.currentIndex)
                        if (!editMode) {
                            // Обычный режим: возвращаем ингредиенты
                            if (!dbWorker.returnIngredients(item.foodId, item.quantity)) {
                                errorDialog.message = "Не удалось вернуть продукты. Удаление отменено."
                                errorDialog.open()
                                return
                            }
                        }
                        orderModel.remove(orderListView.currentIndex)
                        orderListView.currentIndex = -1
                    }
                }
            }

            // Кнопка действия: "Оформить заказ" или "Сохранить изменения"
            Button {
                id: actionButton
                text: editMode ? "Сохранить изменения" : "Оформить заказ"
                Layout.fillWidth: true
                height: 50
                font.pixelSize: 16
                highlighted: true
                Material.background: Material.Green

                onClicked: {
                    if (orderModel.count === 0) {
                        errorDialog.message = "Корзина пуста"
                        errorDialog.open()
                        return
                    }

                    if (editMode) {
                        var items = []
                        for (var i = 0; i < orderModel.count; i++) {
                            var it = orderModel.get(i)
                            items.push({
                                foodId: it.foodId,
                                quantity: it.quantity,
                                price: it.price,
                                name: it.name
                            })
                        }
                        if (dbWorker.syncOrderItems(orderId, items)) {
                            window.dataRefreshRequested()
                            stackView.pop()
                        } else {
                            errorDialog.message = "Ошибка сохранения изменений"
                            errorDialog.open()
                        }
                    } else {
                        var total = 0
                        for (var j = 0; j < orderModel.count; j++) {
                            var item = orderModel.get(j)
                            total += item.price * item.quantity
                        }

                        var userId = dbWorker.getUserId()
                        var newOrderId = dbWorker.createOrder(tableSpinBox.value, userId, requestField.text, total)

                        if (newOrderId > 0) {
                            var success = true
                            for (var k = 0; k < orderModel.count; k++) {
                                var item = orderModel.get(k)
                                if (!dbWorker.addOrderItem(newOrderId, item.foodId, item.quantity, item.price)) {
                                    success = false
                                    break
                                }
                            }
                            if (success) {
                                orderModel.clear()
                                window.dataRefreshRequested()
                                tableSpinBox.value = 1
                                requestField.text = ""
                                errorDialog.message = "Заказ успешно оформлен!"
                                errorDialog.title = "Информация"
                                errorDialog.open()
                            } else {
                                loger.error("Ошибка добавления позиций заказа")
                                errorDialog.message = "Ошибка при добавлении позиций"
                                errorDialog.open()
                            }
                        } else {
                            loger.error("Ошибка создания заказа")
                            errorDialog.message = "Не удалось создать заказ"
                            errorDialog.open()
                        }
                    }
                }
            }
        }
    }
}
