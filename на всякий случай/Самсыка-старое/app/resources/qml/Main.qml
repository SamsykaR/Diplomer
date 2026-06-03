import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

/*!
 * \brief Главное окно приложения.
 *
 * Содержит панель инструментов, боковое меню (Drawer) и стек навигации (StackView).
 * Переключает страницы в зависимости от роли пользователя.
 */
ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    title: "Ресторан - Автоматизация заказов"

    Material.theme: Material.Light
    Material.accent: "#4CAF50"
    Material.primary: "#8B4513"

    //! Псевдоним для доступа к диалогу настроек.
    property alias settingsDialog: settingsDialog

    //! Объект текущего пользователя: { role: "waiter"/"storekeeper"/"admin", name: "..." }.
    property var currentUser: null
    //! Сигнал для запроса обновления данных на активной странице.
    signal dataRefreshRequested()

    /*!
     * \brief Обрабатывает успешный вход.
     * \param role Роль пользователя.
     * \param name Имя пользователя.
     */
    function handleLogin(role, name) {
        print("handleLogin called with role: " + role + ", name: " + name)
        currentUser = { role: role, name: name }
        stackView.clear()
        if (role === "waiter") {
            stackView.push("OrderPage.qml")
        } else if (role === "storekeeper") {
            stackView.push("StockPage.qml")
        } else {
            stackView.push("OrderPage.qml")
        }
    }

    //! Компонент страницы входа.
    Component {
        id: loginComponent
        LoginPage {
            id: loginPageInstance
            Connections {
                target: loginPageInstance
                function onLoginSuccess(role, name) {
                    handleLogin(role, name)
                }
            }
        }
    }

    //! Основной стек навигации.
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginComponent
    }

    //! Диалог настроек.
    Dialog {
        id: settingsDialog
        title: "Настройки"
        modal: true
        width: 800
        height: 560
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: ScrollView {
            clip: true
            RowLayout {
                spacing: 20
                width: parent.width

                // ── Блок База данных ──
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Frame {
                        visible: appSettings.isServerMode
                        Layout.fillWidth: true
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 12
                            Label { text: "База данных"; font.pixelSize: 18; font.bold: true }
                            GridLayout {
                                                            columns: 2; columnSpacing: 12; rowSpacing: 10
                                                            Layout.fillWidth: true
                                                            Label { text: "Хост:" }
                                                            TextField { id: dbHostField; text: appSettings?.dbHost ?? "127.0.0.1"; Layout.fillWidth: true }
                                                            Label { text: "Порт:" }
                                                            SpinBox { id: dbPortSpin; from: 1; to: 65535; value: appSettings?.dbPort ?? 5432; editable: true; Layout.fillWidth: true }
                                                            Label { text: "Имя БД:" }
                                                            TextField { id: dbNameField; text: appSettings?.dbName ?? "RestN"; Layout.fillWidth: true }
                                                            Label { text: "Пользователь:" }
                                                            TextField { id: dbUserField; text: appSettings?.dbUser ?? "R"; Layout.fillWidth: true }
                                                            Label { text: "Пароль:" }
                                                            TextField { id: dbPasswordField; text: appSettings?.dbPassword ?? ""; echoMode: TextInput.Password; Layout.fillWidth: true }
                                                        }
                            Button {
                                text: "Проверить подключение"
                                Layout.fillWidth: true
                                onClicked: {
                                    var ok = dbWorker.reconnect(dbHostField.text, dbPortSpin.value, dbNameField.text, dbUserField.text, dbPasswordField.text)
                                    statusLabel.text = ok ? "Подключено успешно" : "Ошибка подключения"
                                    statusLabel.color = ok ? "green" : "red"
                                }
                            }
                            Label { id: statusLabel; text: ""; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                        }
                    }
                }

                // ── Блок Сеть ──
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Frame {
                        Layout.fillWidth: true
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 12
                            Label { text: "Сеть"; font.pixelSize: 18; font.bold: true }
                            GridLayout {
                                                            columns: 2; columnSpacing: 12; rowSpacing: 10
                                                            Layout.fillWidth: true
                                                            Label { text: "IP сервера:" }
                                                            TextField { id: serverHostField; text: appSettings.serverHost; Layout.fillWidth: true }
                                                            Label { text: "Порт:" }
                                                            TextField {
                                                                id: serverPortField;
                                                                text: appSettings.serverPort;
                                                                validator: IntValidator { bottom: 1024; top: 65535 }
                                                                Layout.fillWidth: true }
                                                        }
                                                        CheckBox {
                                                            id: serverModeCheck
                                                            text: "Режим сервера"
                                                            checked: appSettings?.isServerMode ?? false
                                                            onCheckedChanged: if (appSettings) appSettings.isServerMode = checked
                                                        }
                            Button {
                                text: "Проверить сервер"
                                Layout.fillWidth: true
                                onClicked: {
                                    var host = serverHostField.text
                                    var port = parseInt(serverPortField.text)
                                    var ok = dbWorker.pingServer(host, port)
                                    serverStatusLabel.text = ok ? "Сервер доступен" : "Сервер не отвечает"
                                    serverStatusLabel.color = ok ? "green" : "red"
                                }
                            }
                            Label { id: serverStatusLabel; text: ""; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                                                        Button {
                                                            text: "Показать IP устройства"
                                                            Layout.fillWidth: true
                                                            onClicked: {
                                                                var ips = dbWorker.getLocalIps()
                                                                var port = parseInt(serverPortField.text)
                                                                var text = ""
                                                                for (var i = 0; i < ips.length; i++) {
                                                                    text += ips[i] + ":" + port
                                                                    if (i < ips.length - 1) text += ", "
                                                                }
                                                                ipLabel.text = text ? text : "Нет доступных IP"
                                                            }
                                                        }
                                                        Label { id: ipLabel; text: ""; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                                                    }
                                                }
                                            }

                // ── Блок Прочее ──
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Frame {
                        Layout.fillWidth: true
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 12
                            Label { text: "Прочее"; font.pixelSize: 18; font.bold: true }
                            GridLayout {
                                                            columns: 2; columnSpacing: 12; rowSpacing: 10
                                                            Layout.fillWidth: true
                                                            Label { text: "Путь к логам:" }
                                                            TextField { id: logPathField; text: appSettings?.logPath ?? "logs.txt"; Layout.fillWidth: true }
                                                            Label { text: "Папка для отчётов:" }
                                                            TextField {
                                                                id: reportsPathField
                                                                text: appSettings ? appSettings.reportsPath : ""
                                                                Layout.fillWidth: true
                                                            }
                        }
                    }
                }
            }
        }
        }

        footer: DialogButtonBox {
            standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
            onAccepted: {
                // Сохранение настроек и вызов reconnect
                if (appSettings) {
                    appSettings.logPath = logPathField.text
                    appSettings.dbHost = dbHostField.text
                    appSettings.dbPort = dbPortSpin.value
                    appSettings.dbName = dbNameField.text
                    appSettings.dbUser = dbUserField.text
                    appSettings.dbPassword = dbPasswordField.text
                    appSettings.serverHost = serverHostField.text
                    appSettings.serverPort = parseInt(serverPortField.text)
                    loger.setLogPath(logPathField.text)
                    appSettings.reportsPath = reportsPathField.text
                    appSettings.save()
                    loger.info("Вызов reconnect с хостом "+ serverHostField.text + " порт" + parseInt(serverPortField.text))
                    dbWorker.reconnect(serverHostField.text, parseInt(serverPortField.text))
                }
                settingsDialog.close()
            }
            onRejected: settingsDialog.close()
        }
    }

    // ── Панель инструментов ──
    header: ToolBar {
        visible: currentUser !== null
        Material.background: Material.primary

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            // Кнопка гамбургер-меню
            ToolButton {
                id: menuButton
                contentItem: Label {
                    text: "☰"
                    font.pixelSize: 28
                    color: "white"
                }
                onClicked: drawer.open()
            }

            // Заголовок
            Label {
                text: "Ресторан"
                font.pixelSize: 22
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Кнопка синхронизации
            ToolButton {
                id: syncButton
                contentItem: Label {
                    text: "⟳"
                    font.pixelSize: 24
                    color: "white"
                }
                onClicked: {
                    loger.info("Синхронизация данных...")
                    dataRefreshRequested()
                    loger.info("Синхронизация данных завершена")
                }
            }
        }
    }

    // ── Боковое меню ──
    Drawer {
        id: drawer
        width: 300
        height: parent.height
        modal: true
        dragMargin: 0
        interactive: currentUser !== null

        background: Rectangle { color: "#F0E6D2" }

        ColumnLayout {
            anchors.fill: parent
            spacing: 2

            // Имя пользователя
            Rectangle {
                Layout.fillWidth: true
                height: 80
                color: "#E0D6C2"
                Label {
                    anchors.centerIn: parent
                    text: currentUser ? currentUser.name : ""
                    font.pixelSize: 20
                    font.bold: true
                }
            }

            // Пункты меню в зависимости от роли
            Repeater {
                model: {
                    if (!currentUser) return []
                    if (currentUser.role === "admin") {
                        return [
                            { name: "Оформление заказа", page: "OrderPage.qml" },
                            { name: "Склад", page: "StockPage.qml" },
                            { name: "Отчёты", page: "ReportsPage.qml" }
                        ]
                    } else if (currentUser.role === "waiter") {
                        return [ { name: "Оформление заказа", page: "OrderPage.qml" } ]
                    } else if (currentUser.role === "storekeeper") {
                        return [ { name: "Склад", page: "StockPage.qml" } ]
                    }
                    return []
                }
                delegate: Button {
                    text: modelData.name
                    Layout.fillWidth: true
                    height: 60
                    font.pixelSize: 18
                    flat: true
                    onClicked: {
                        stackView.replace(modelData.page)
                        drawer.close()
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Настройки
            Button {
                text: "Настройки"
                Layout.fillWidth: true
                height: 60
                font.pixelSize: 18
                flat: true
                onClicked: {
                    settingsDialog.open()
                    drawer.close()
                }
            }

            // Выход
            Button {
                text: "Выйти"
                Layout.fillWidth: true
                height: 60
                font.pixelSize: 18
                flat: true
                onClicked: {
                    loger.info("Выход из учётной записи")
                    currentUser = null
                    stackView.replace(loginComponent)
                    drawer.close()
                }
            }

            // Закрыть приложение
            Button {
                text: "Закрыть приложение"
                Layout.fillWidth: true
                height: 60
                font.pixelSize: 18
                flat: true
                onClicked: {
                    loger.info("Закрытие приложения");
                    Qt.quit()
                }
            }
        }
    }
}

