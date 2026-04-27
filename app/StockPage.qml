import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    title: "Склад"

    ListModel { id: stockModel }
    property int selectedProductIndex: -1

    function loadProducts() {
        var products = dbWorker.getProducts()
        stockModel.clear()
        for (var i = 0; i < products.length; i++) {
            stockModel.append(products[i])
        }
    }

    function reloadData() {
            print("StockPage: reloading products")
            loadProducts()
        }

        Connections {
            target: window
            function onDataRefreshRequested() {
                reloadData()
            }
        }

    Component.onCompleted: loadProducts()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        ListView {
            id: productListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: stockModel
            delegate: ItemDelegate {
                width: parent ? parent.width : 0
                height: 60
                highlighted: ListView.isCurrentItem

                background: Rectangle {
                    color: model.quantity < model.minThreshold ? "#FFCDD2" : "transparent"
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    Label {
                        text: model.name
                        font.pixelSize: 18
                        Layout.fillWidth: true
                    }
                    Label {
                        text: model.quantity + " " + model.unit
                        font.pixelSize: 16
                        color: "#555"
                        Layout.alignment: Qt.AlignRight
                    }
                }

                onClicked: {
                    productListView.currentIndex = index
                    selectedProductIndex = index
                }
            }
        }

        GroupBox {
            title: "Изменение остатка"
            Layout.fillWidth: true
            enabled: selectedProductIndex >= 0

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                RowLayout {
                    Label { text: "Продукт:"; font.bold: true }
                    Label { text: selectedProductIndex >= 0 ? stockModel.get(selectedProductIndex).name : "" }
                }

                TextField {
                    id: deltaField
                    Layout.fillWidth: true
                    placeholderText: "Количество"
                    validator: IntValidator { bottom: 0; top: 10000 }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Button {
                        text: "Добавить"
                        Layout.fillWidth: true
                        Material.background: Material.Green
                        onClicked: {
                            if (selectedProductIndex < 0) return
                            var delta = parseInt(deltaField.text) || 0
                            if (delta <= 0) return
                            var product = stockModel.get(selectedProductIndex)
                            if (dbWorker.addProductQuantity(product.productId, delta)) {
                                product.quantity += delta
                                stockModel.setProperty(selectedProductIndex, "quantity", product.quantity)
                                deltaField.clear()
                            }
                        }
                    }

                    Button {
                        text: "Убавить"
                        Layout.fillWidth: true
                        Material.background: Material.Red
                        onClicked: {
                            if (selectedProductIndex < 0) return
                            var delta = parseInt(deltaField.text) || 0
                            if (delta <= 0) return
                            var product = stockModel.get(selectedProductIndex)
                            if (product.quantity - delta >= 0) {
                                if (dbWorker.addProductQuantity(product.productId, -delta)) {
                                    product.quantity -= delta
                                    stockModel.setProperty(selectedProductIndex, "quantity", product.quantity)
                                    deltaField.clear()
                                }
                            }
                        }
                    }

                    Button {
                        text: "Установить"
                        Layout.fillWidth: true
                        Material.background: "#8B4513"
                        onClicked: {
                            if (selectedProductIndex < 0) return
                            var newVal = parseInt(deltaField.text) || 0
                            var product = stockModel.get(selectedProductIndex)
                            if (dbWorker.updateProductQuantity(product.productId, newVal)) {
                                product.quantity = newVal
                                stockModel.setProperty(selectedProductIndex, "quantity", product.quantity)
                                deltaField.clear()
                            }
                        }
                    }
                }
            }
        }
    }
}
