import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.alignment: Qt.AlignTop
    spacing: 10

    SourceImageFileSelector {
        id: rgb2GbrSourceImageSelector
        objectName: "rgb2GbrSourceImageSelector"
        Layout.fillWidth: true
    }

    ResultLocationSelector {
        id: rgb2GbrImageResultLocationSelector
        objectName: "rgb2GbrImageResultLocationSelector"
        Layout.fillWidth: true
    }

    Button {
        id: btnSendRgb2Gbr
        objectName: "btnSendRgb2Gbr"
        text: qsTr("Publish Job")
        onClicked: uicore.doSendJobRgb2Gbr()
    }
}
