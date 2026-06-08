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
        width: Math.min(1000, parent.width * 0.9)
        height: Math.min(700, parent.height * 0.85)
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Material.theme: Material.Light
        Material.accent: Material.Blue

        contentItem: ScrollView {
            id: settingsScroll
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded   // горизонтальный скролл при необходимости
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            RowLayout {
                // Ширина контейнера берётся как максимум из видимой области и суммарной потребности фреймов.
                // Благодаря этому, когда фреймы не помещаются, появляется горизонтальная прокрутка,
                // а если помещаются – они займут всю доступную ширину без лишнего скролла.
                width: Math.max(settingsScroll.availableWidth, implicitWidth)
                height: settingsScroll.availableHeight
                spacing: 16

                // Группа База данных (видна только в серверном режиме)
                Frame {
                    visible: appSettings ? appSettings.isServerMode : false
                    Layout.preferredWidth: 380        // желаемая ширина
                    Layout.minimumWidth: 340           // минимальная ширина (чтобы не сжимался слишком сильно)
                    Layout.fillWidth: false            // не растягиваться, если место остаётся
                    Layout.fillHeight: true
                    background: Rectangle { color: "#F8F9FA"; radius: 8; border.color: "#E0E0E0" }
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        Label { text: "База данных"; font.bold: true; font.pixelSize: 16 }
                        GridLayout {
                            columns: 2
                            columnSpacing: 16
                            rowSpacing: 12
                            Layout.fillWidth: true
                            Label { text: "Хост:"; font.pixelSize: 14 }
                            TextField { id: dbHostField; text: appSettings?.dbHost ?? "127.0.0.1"; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Порт:"; font.pixelSize: 14 }
                            SpinBox { id: dbPortSpin; from: 1; to: 65535; value: appSettings?.dbPort ?? 5432; editable: true; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Имя БД:"; font.pixelSize: 14 }
                            TextField { id: dbNameField; text: appSettings?.dbName ?? "RestN"; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Пользователь:"; font.pixelSize: 14 }
                            TextField { id: dbUserField; text: appSettings?.dbUser ?? "R"; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Пароль:"; font.pixelSize: 14 }
                            TextField { id: dbPasswordField; text: appSettings?.dbPassword ?? ""; echoMode: TextInput.Password; Layout.fillWidth: true; font.pixelSize: 14 }
                        }
                        RowLayout {
                            Button {
                                text: "Проверить подключение"
                                onClicked: {
                                    var ok = dbWorker.reconnect(dbHostField.text, dbPortSpin.value, dbNameField.text, dbUserField.text, dbPasswordField.text)
                                    statusLabel.text = ok ? "Подключено успешно" : "Ошибка подключения"
                                    statusLabel.color = ok ? "green" : "red"
                                }
                            }
                            Label { id: statusLabel; text: ""; font.pixelSize: 12; Layout.fillWidth: true }
                        }
                    }
                }

                // Группа Сеть
                Frame {
                    Layout.preferredWidth: 380
                    Layout.minimumWidth: 340
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    background: Rectangle { color: "#F8F9FA"; radius: 8; border.color: "#E0E0E0" }
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        Label { text: "Сеть"; font.bold: true; font.pixelSize: 16 }
                        GridLayout {
                            columns: 2
                            columnSpacing: 16
                            rowSpacing: 12
                            Layout.fillWidth: true
                            Label { text: "IP сервера:"; font.pixelSize: 14 }
                            TextField { id: serverHostField; text: appSettings ? appSettings.serverHost : ""; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Порт сервера:"; font.pixelSize: 14 }
                            TextField {
                                id: serverPortField
                                text: appSettings ? appSettings.serverPort : ""
                                validator: IntValidator { bottom: 1024; top: 65535 }
                                Layout.fillWidth: true
                                font.pixelSize: 14
                            }
                            Item { Layout.columnSpan: 2; Layout.fillWidth: true }
                            CheckBox {
                                id: serverModeCheck
                                text: "Режим сервера"
                                checked: appSettings ? appSettings.isServerMode : false
                                onCheckedChanged: if (appSettings) appSettings.isServerMode = checked
                                font.pixelSize: 14
                            }
                        }
                        RowLayout {
                            Button {
                                text: "Проверить сервер"
                                onClicked: {
                                    var host = serverHostField.text
                                    var port = parseInt(serverPortField.text)
                                    var ok = dbWorker.pingServer(host, port)
                                    serverStatusLabel.text = ok ? "Сервер доступен" : "Сервер не отвечает"
                                    serverStatusLabel.color = ok ? "green" : "red"
                                }
                            }
                            Label { id: serverStatusLabel; text: ""; font.pixelSize: 12; Layout.fillWidth: true }
                        }
                        RowLayout {
                            Button {
                                text: "Показать IP устройства"
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
                            Label { id: ipLabel; text: ""; font.pixelSize: 12; Layout.fillWidth: true; wrapMode: Text.WordWrap }
                        }
                    }
                }

                // Группа Прочее
                Frame {
                    Layout.preferredWidth: 380
                    Layout.minimumWidth: 340
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    background: Rectangle { color: "#F8F9FA"; radius: 8; border.color: "#E0E0E0" }
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        Label { text: "Прочее"; font.bold: true; font.pixelSize: 16 }
                        GridLayout {
                            columns: 2
                            columnSpacing: 16
                            rowSpacing: 12
                            Layout.fillWidth: true
                            Label { text: "Путь к логам:"; font.pixelSize: 14 }
                            TextField { id: logPathField; text: appSettings?.logPath ?? "logs.txt"; Layout.fillWidth: true; font.pixelSize: 14 }
                            Label { text: "Папка для отчётов:"; font.pixelSize: 14 }
                            TextField {
                                id: reportsPathField
                                text: appSettings ? appSettings.reportsPath : ""
                                Layout.fillWidth: true
                                font.pixelSize: 14
                            }
                        }
                    }
                }
            }
        }

        footer: DialogButtonBox {
            // Убрано дублирование кнопок (удалено standardButtons)
            onAccepted: {
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
                    dbWorker.updateConnectionParams(dbHostField.text, dbPortSpin.value, dbNameField.text, dbUserField.text, dbPasswordField.text)
                    loger.info("Вызов reconnect с хостом "+ serverHostField.text + " порт" + parseInt(serverPortField.text))
                    dbWorker.reconnect(serverHostField.text, parseInt(serverPortField.text))
                }
                settingsDialog.close()
            }
            onRejected: settingsDialog.close()

            Button {
                flat: true
                font.pixelSize: 18
                text: "OK"
                onClicked: settingsDialog.accept()
            }
            Button {
                flat: true
                font.pixelSize: 18
                text: "Отмена"
                onClicked: settingsDialog.reject()
            }
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
                            { name: "Отчёты", page: "ReportsPage.qml" },
                            { name: "Активные заказы", page: "ActiveOrdersPage.qml" }
                        ]
                    } else if (currentUser.role === "waiter") {
                        return [
                            { name: "Оформление заказа", page: "OrderPage.qml" },
                            { name: "Активные заказы", page: "ActiveOrdersPage.qml" }
                        ]
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

