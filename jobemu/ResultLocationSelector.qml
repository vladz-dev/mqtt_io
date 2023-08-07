import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

RowLayout
{
    Layout.fillWidth: true

    TextField {
        id: txtResultImageLocation
        Layout.alignment: Qt.AlignLeft
        Layout.fillWidth: true
    }

    FolderDialog {
        id: resultLocationFolderDialog
        currentFolder: txtResultImageLocation
        onAccepted: {
            txtResultImageLocation.text = resultLocationFolderDialog.selectedFolder
            resultLocationFolderDialog.close()
        }
    }

    Button {
        id: btnBrowseResultImageLocation
        Layout.alignment: Qt.AlignRight
        text: qsTr("Result Image Location")
        onClicked: {
            resultLocationFolderDialog.open()
        }
    }
}
