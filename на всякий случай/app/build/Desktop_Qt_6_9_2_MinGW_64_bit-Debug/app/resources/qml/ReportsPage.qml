import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Labs

Page {
    title: "Отчёты"

    property int selectedReportIndex: 0

    ListModel { id: dishModel }
    ListModel { id: waiterModel }
    property int selectedDishId: 0
    property int selectedTable: 0
    property int selectedWaiterId: 0

    ListModel {
        id: reportTypes
        ListElement { name: "Выручка"; type: "revenue" }
        ListElement { name: "Популярность блюд"; type: "popularity" }
        ListElement { name: "Малые остатки"; type: "lowstock" }
        ListElement { name: "Чеки"; type: "checks" }
    }

    function loadFilterData() {
        dishModel.clear()
        dishModel.append({ id: 0, name: "Все блюда" })
        var dishes = dbWorker.getDishesList()
        for (var i = 0; i < dishes.length; i++) dishModel.append(dishes[i])

        waiterModel.clear()
        waiterModel.append({ id: 0, name: "Все официанты" })
        var waiters = dbWorker.getWaitersList()
        for (var i = 0; i < waiters.length; i++) waiterModel.append(waiters[i])
    }

    Dialog {
        id: checkDialog
        title: "Чек заказа №" + (checkDialog.orderNumber || "")
        modal: true
        width: 550
        height: 450
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        property int orderId: 0
        property int orderNumber: 0

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

    Component.onCompleted: loadFilterData()

    Connections {
        target: window
        function onDataRefreshRequested() { loadFilterData() }
    }

    Labs.FileDialog {
        id: saveFileDialog
        title: "Сохранить отчёт"
        fileMode: Labs.FileDialog.SaveFile
        defaultSuffix: "csv"
        folder: "file:///" + (appSettings ? appSettings.reportsPath : "")
        onAccepted: {
            var fullPath = saveFileDialog.file.toString().replace("file:///", "")
            if (selectedReportIndex === 0) {
                var from = fromDateField.text.trim()
                var to = toDateField.text.trim()
                if (from === "" || to === "") { resultsText.text = "Введите даты"; return }
                if (dbWorker.saveReportToCSV("revenue", from, to, selectedDishId, selectedTable, selectedWaiterId, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else resultsText.text = "Ошибка сохранения"
            } else if (selectedReportIndex === 1) {
                var from = popFromDateField.text.trim()
                var to = popToDateField.text.trim()
                if (from === "" || to === "") { resultsText.text = "Введите даты"; return }
                if (dbWorker.saveReportToCSV("popularity", from, to, 0, 0, 0, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else resultsText.text = "Ошибка сохранения"
            } else if (selectedReportIndex === 2) {
                if (dbWorker.saveReportToCSV("lowstock", "", "", 0, 0, 0, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else resultsText.text = "Ошибка сохранения"
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        ComboBox {
            id: reportCombo
            Layout.fillWidth: true
            model: reportTypes
            textRole: "name"
            onCurrentIndexChanged: selectedReportIndex = currentIndex
        }

        // Выручка
        ColumnLayout {
            Layout.fillWidth: true
            visible: selectedReportIndex === 0
            spacing: 10
            RowLayout { Label { text: "Дата с (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: fromDateField; Layout.fillWidth: true; placeholderText: "2026-01-01" } }
            RowLayout { Label { text: "Дата по (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: toDateField; Layout.fillWidth: true; placeholderText: "2026-12-31" } }
            RowLayout { Label { text: "Блюдо:"; Layout.preferredWidth: 120 }
                        ComboBox { id: dishCombo; Layout.fillWidth: true; model: dishModel; textRole: "name"
                                   onCurrentIndexChanged: selectedDishId = model.get(currentIndex).id } }
            RowLayout { Label { text: "Столик:"; Layout.preferredWidth: 120 }
                        SpinBox { id: tableSpinBox; from: 0; to: 50; value: 0; editable: true; Layout.fillWidth: true
                                 onValueChanged: selectedTable = value } }
            RowLayout { Label { text: "Официант:"; Layout.preferredWidth: 120 }
                        ComboBox { id: waiterCombo; Layout.fillWidth: true; model: waiterModel; textRole: "name"
                                   onCurrentIndexChanged: selectedWaiterId = model.get(currentIndex).id } }
        }

        // Популярность блюд
        ColumnLayout {
            Layout.fillWidth: true
            visible: selectedReportIndex === 1
            spacing: 10
            RowLayout { Label { text: "Дата с (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: popFromDateField; Layout.fillWidth: true; placeholderText: "2026-01-01" } }
            RowLayout { Label { text: "Дата по (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: popToDateField; Layout.fillWidth: true; placeholderText: "2026-12-31" } }
        }

        // Малые остатки
        Label {
            Layout.fillWidth: true
            visible: selectedReportIndex === 2
            text: "Отображаются продукты, количество которых ниже их минимального порога (MinThreshold)"
            font.pixelSize: 12; color: Material.secondaryTextColor; wrapMode: Text.WordWrap
        }

        // === Чеки с фильтром по датам ===
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: selectedReportIndex === 3
            spacing: 10

            RowLayout {
                Label { text: "Дата с (ГГГГ-ММ-ДД):" }
                TextField { id: checksFromDate; placeholderText: "2026-01-01" }
                Label { text: "по:" }
                TextField { id: checksToDate; placeholderText: "2026-12-31" }
                Button {
                    text: "Загрузить чеки"
                    onClicked: {
                        var from = checksFromDate.text.trim()
                        var to = checksToDate.text.trim()
                        if (from === "" || to === "") {
                            checksErrorLabel.text = "Введите даты в формате ГГГГ-ММ-ДД"
                            return
                        }
                        checksErrorLabel.text = ""
                        var checks = dbWorker.getChecksByDateRange(from, to)
                        checksModel.clear()
                        for (var i = 0; i < checks.length; i++) checksModel.append(checks[i])
                        if (checks.length === 0) checksErrorLabel.text = "Нет чеков за указанный период"
                    }
                }
            }
            Label { id: checksErrorLabel; color: "red"; font.pixelSize: 12; visible: text !== "" }

            ListView {
                id: checksListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: ListModel { id: checksModel }
                delegate: Rectangle {
                    width: checksListView.width
                    height: 70
                    color: index % 2 === 0 ? "#F5F5F5" : "white"
                    border.color: "#DDD"
                    radius: 4
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Label { text: "Чек №" + model.checkNumber; font.bold: true; Layout.preferredWidth: 130; elide: Text.ElideRight }
                        Label { text: model.totalSum.toFixed(2) + " ₽"; Layout.preferredWidth: 80 }
                        Label { text: "Стол " + model.tableNumber; Layout.preferredWidth: 60 }
                        Label { text: model.waiterName || "?"; Layout.preferredWidth: 100; elide: Text.ElideRight }
                        Label { text: new Date(model.createdAt).toLocaleDateString(); Layout.preferredWidth: 90 }
                        Button { text: "Просмотр"; onClicked: { checkDialog.loadCheck(model.orderId, model.orderNumber); checkDialog.open() } }
                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }

        // Область вывода результатов для обычных отчётов
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: selectedReportIndex !== 3
            contentHeight: resultsText.implicitHeight
            clip: true
            TextArea {
                id: resultsText
                width: parent.width
                text: "Выберите отчёт и нажмите 'Сформировать'"
                font.pixelSize: 14
                readOnly: true
                wrapMode: TextEdit.Wrap
            }
        }

        // Кнопки управления
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            visible: selectedReportIndex !== 3
            spacing: 20
            Button {
                text: "Сформировать"
                Layout.preferredWidth: 200; height: 50; font.pixelSize: 16
                highlighted: true; Material.background: Material.Green
                onClicked: {
                    resultsText.text = ""
                    if (selectedReportIndex === 0) {
                        var from = fromDateField.text.trim()
                        var to = toDateField.text.trim()
                        if (from === "" || to === "") { resultsText.text = "Введите даты"; return }
                        var totalReport = dbWorker.getReportRevenue(from, to, selectedDishId, selectedTable, selectedWaiterId)
                        if (totalReport.error) { resultsText.text = "Ошибка: " + totalReport.error; return }
                        var text = "Общая выручка: " + totalReport.totalRevenue.toFixed(2) + " руб.\n\n"
                        var days = dbWorker.getRevenueByDay(from, to, selectedDishId, selectedTable, selectedWaiterId)
                        if (days.length === 0) text += "Нет заказов за указанный период"
                        else {
                            text += "Выручка по дням:\n"
                            for (var i = 0; i < days.length; i++)
                                text += days[i].date + ": " + days[i].total.toFixed(2) + " руб.\n"
                        }
                        resultsText.text = text
                    } else if (selectedReportIndex === 1) {
                        var from = popFromDateField.text.trim()
                        var to = popToDateField.text.trim()
                        if (from === "" || to === "") { resultsText.text = "Введите даты"; return }
                        var popularity = dbWorker.getFoodPopularity(from, to)
                        if (popularity.length === 0) resultsText.text = "Нет заказов за указанный период"
                        else {
                            var text = "Популярность блюд:\n"
                            for (var i = 0; i < popularity.length; i++)
                                text += "• " + popularity[i].foodName + ": " + popularity[i].count + " порц.\n"
                            resultsText.text = text
                        }
                    } else if (selectedReportIndex === 2) {
                        var lowStock = dbWorker.getReportLowStock()
                        if (lowStock.length === 0) resultsText.text = "Все продукты в наличии выше минимального порога"
                        else {
                            var text = "Продукты ниже минимального порога:\n"
                            for (var i = 0; i < lowStock.length; i++)
                                text += "• " + lowStock[i].name + ": " + lowStock[i].quantity + " (мин. " + lowStock[i].minThreshold + ")\n"
                            resultsText.text = text
                        }
                    }
                }
            }
            Button {
                text: "Сохранить отчёт"
                Layout.preferredWidth: 200; height: 50; font.pixelSize: 16
                onClicked: saveFileDialog.open()
            }
        }
    }
}
