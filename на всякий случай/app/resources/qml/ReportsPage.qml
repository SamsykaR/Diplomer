import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Labs

/*!
 * \brief Страница отчётов.
 *
 * Позволяет формировать отчёты по выручке, популярности блюд
 * и малым остаткам. Результаты отображаются в текстовой области,
 * их можно сохранить в CSV-файл.
 */
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
    }

    /*!
     * \brief Загружает списки блюд и официантов для фильтров.
     */
    function loadFilterData() {
        dishModel.clear()
        dishModel.append({ id: 0, name: "Все блюда" })
        var dishes = dbWorker.getDishesList()
        for (var i = 0; i < dishes.length; i++) {
            dishModel.append(dishes[i])
        }

        waiterModel.clear()
        waiterModel.append({ id: 0, name: "Все официанты" })
        var waiters = dbWorker.getWaitersList()
        for (var i = 0; i < waiters.length; i++) {
            waiterModel.append(waiters[i])
        }
    }

    Component.onCompleted: loadFilterData()

    Connections {
        target: window
        function onDataRefreshRequested() {
            loadFilterData()
        }
    }

    // Диалог сохранения файла
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
                if (from === "" || to === "") {
                    resultsText.text = "Введите даты в формате ГГГГ-ММ-ДД"
                    return
                }
                if (dbWorker.saveReportToCSV("revenue", from, to, selectedDishId, selectedTable, selectedWaiterId, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else
                    resultsText.text = "Ошибка сохранения отчёта"
            }
            else if (selectedReportIndex === 1) {
                var from = popFromDateField.text.trim()
                var to = popToDateField.text.trim()
                if (from === "" || to === "") {
                    resultsText.text = "Введите даты в формате ГГГГ-ММ-ДД"
                    return
                }
                if (dbWorker.saveReportToCSV("popularity", from, to, 0, 0, 0, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else
                    resultsText.text = "Ошибка сохранения отчёта"
            }
            else if (selectedReportIndex === 2) {
                if (dbWorker.saveReportToCSV("lowstock", "", "", 0, 0, 0, fullPath))
                    resultsText.text = "Отчёт сохранён в " + fullPath
                else
                    resultsText.text = "Ошибка сохранения отчёта"
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

        // Блок фильтров для выручки
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

        // Блок фильтров для популярности
        ColumnLayout {
            Layout.fillWidth: true
            visible: selectedReportIndex === 1
            spacing: 10
            RowLayout { Label { text: "Дата с (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: popFromDateField; Layout.fillWidth: true; placeholderText: "2026-01-01" } }
            RowLayout { Label { text: "Дата по (ГГГГ-ММ-ДД):"; Layout.preferredWidth: 120 }
                        TextField { id: popToDateField; Layout.fillWidth: true; placeholderText: "2026-12-31" } }
        }

        // Пояснение для малых остатков
        Label {
            Layout.fillWidth: true
            visible: selectedReportIndex === 2
            text: "Отображаются продукты, количество которых ниже их минимального порога (MinThreshold)"
            font.pixelSize: 12
            color: Material.secondaryTextColor
            wrapMode: Text.WordWrap
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20

            // Кнопка "Сформировать"
            Button {
                text: "Сформировать"
                Layout.preferredWidth: 200
                height: 50
                font.pixelSize: 16
                highlighted: true
                Material.background: Material.Green
                onClicked: {
                    resultsText.text = ""

                    if (selectedReportIndex === 0) {
                        var from = fromDateField.text.trim()
                        var to = toDateField.text.trim()
                        if (from === "" || to === "") {
                            resultsText.text = "Введите даты в формате ГГГГ-ММ-ДД"
                            return
                        }
                        var totalReport = dbWorker.getReportRevenue(from, to, selectedDishId, selectedTable, selectedWaiterId)
                        if (totalReport.error) {
                            resultsText.text = "Ошибка: " + totalReport.error
                            return
                        }
                        var text = "Общая выручка: " + totalReport.totalRevenue.toFixed(2) + " руб.\n\n"
                        var days = dbWorker.getRevenueByDay(from, to, selectedDishId, selectedTable, selectedWaiterId)
                        if (days.length === 0) {
                            text += "Нет заказов за указанный период"
                        } else {
                            text += "Выручка по дням:\n"
                            for (var i = 0; i < days.length; i++) {
                                text += days[i].date + ": " + days[i].total.toFixed(2) + " руб.\n"
                            }
                        }
                        resultsText.text = text
                    }
                    else if (selectedReportIndex === 1) {
                        var from = popFromDateField.text.trim()
                        var to = popToDateField.text.trim()
                        if (from === "" || to === "") {
                            resultsText.text = "Введите даты в формате ГГГГ-ММ-ДД"
                            return
                        }
                        var popularity = dbWorker.getFoodPopularity(from, to)
                        if (popularity.length === 0) {
                            resultsText.text = "Нет заказов за указанный период"
                        } else {
                            var text = "Популярность блюд:\n"
                            for (var i = 0; i < popularity.length; i++) {
                                text += "• " + popularity[i].foodName + ": " + popularity[i].count + " порц.\n"
                            }
                            resultsText.text = text
                        }
                    }
                    else if (selectedReportIndex === 2) {
                        var lowStock = dbWorker.getReportLowStock()
                        if (lowStock.length === 0) {
                            resultsText.text = "Все продукты в наличии выше минимального порога"
                        } else {
                            var text = "Продукты ниже минимального порога:\n"
                            for (var i = 0; i < lowStock.length; i++) {
                                text += "• " + lowStock[i].name + ": " + lowStock[i].quantity +
                                        " (мин. " + lowStock[i].minThreshold + ")\n"
                            }
                            resultsText.text = text
                        }
                    }
                }
            }
            // Кнопка "Сохранить отчёт"
                        Button {
                            text: "Сохранить отчёт"
                            Layout.preferredWidth: 200
                            height: 50
                            font.pixelSize: 16
                            highlighted: false
                            Material.background: Material.Blue
                            onClicked: {
                                saveFileDialog.open()
                            }
                        }
                    }

                    // Область вывода результата
                    Flickable {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
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
                }
            }
