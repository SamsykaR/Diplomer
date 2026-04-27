import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: loginPage
    signal loginSuccess(string role, string name)

    background: Rectangle { color: "#FAFAFA" }

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(400, parent.width * 0.8)
        spacing: 20

        Label {
            text: "Вход в систему"
            font.pixelSize: 28
            font.bold: true
            color: Material.primary
            Layout.alignment: Qt.AlignHCenter
        }

        TextField {
            id: loginField
            Layout.fillWidth: true
            placeholderText: "Логин"
            font.pixelSize: 18
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            placeholderText: "Пароль"
            font.pixelSize: 18
            echoMode: TextInput.Password
        }

        Button {
            text: "Войти"
            Layout.fillWidth: true
            font.pixelSize: 18
            highlighted: true
            Material.background: Material.Green
            onClicked: {
                errorLabel.visible = false
                var success = dbWorker.checkUser(loginField.text, passwordField.text)
                if (success) {
                    var role = dbWorker.getUserRole()
                    var name = dbWorker.getUserName()
                    loger.info("QML: Логин: role=" + role + ", name=" + name)
                    loginSuccess(role, name)
                } else {
                    errorLabel.visible = true
                    loginField.text = ""
                    passwordField.text = ""
                }
            }
        }

        // Кнопка настроек
        Button {
            text: "Настройки"
            Layout.fillWidth: true
            font.pixelSize: 18
            onClicked: {
                if (window.settingsDialog) {
                    window.settingsDialog.open()
                } else {

                }
            }
        }

        Label {
            id: errorLabel
            text: "Неверный логин или пароль"
            color: "red"
            font.pixelSize: 14
            visible: false
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
