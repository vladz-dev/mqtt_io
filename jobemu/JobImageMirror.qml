import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.alignment: Qt.AlignTop
    spacing: 10

    SourceImageFileSelector {
        id: mirrorSourceImageSelector
        objectName: "mirrorSourceImageSelector"
        Layout.fillWidth: true
    }

    ResultLocationSelector {
        id: mirrorImageResultLocationSelector
        objectName: "mirrorImageResultLocationSelector"
        Layout.fillWidth: true
    }

    RowLayout {
        CheckBox {
            id: chkboxMirrorVertical
            objectName: "chkboxMirrorVertical"
            text: qsTr("Mirror Vertical")
        }
        CheckBox {
            id: chkboxMirrorHorizontal
            objectName: "chkboxMirrorHorizontal"
            text: qsTr("Mirror Horizontal")
        }
    }

    Button {
        id: btnSendMirror
        objectName: "btnSendMirror"
        text: qsTr("Publish Job")
        onClicked: uicore.doSendJobMirror()
    }
}
