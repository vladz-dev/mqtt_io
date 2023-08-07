import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

RowLayout
{
    Layout.fillWidth: true

    FileDialog {
        id: sourceFileDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: 1
        nameFilters: ["JPEG files (*.jpreg *.jpg)", "GIF files (*.gif)", "PNG files (*.png)"]
        currentFile: txtSourceImage
        onAccepted: {
            txtSourceImage.text = sourceFileDialog.selectedFile
            sourceFileDialog.close()
        }
    }

    TextField {
        id: txtSourceImage
        objectName: "sourceImage"
        Layout.alignment: Qt.AlignLeft
        Layout.fillWidth: true
        //text: sourceFileDialog.file
    }

    Button {
        id: btnBrowseImage
        Layout.alignment: Qt.AlignRight
        text: qsTr("Source Image")
        onClicked: {
            sourceFileDialog.open()
        }
    }
}
