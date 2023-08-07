import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.alignment: Qt.AlignTop
    spacing: 10

    SourceImageFileSelector {
        id: subRectSourceImageSelector
        objectName: "subRectSourceImageSelector"
        Layout.fillWidth: true
    }

    ResultLocationSelector {
        id: subRectImageResultLocationSelector
        objectName: "subRectImageResultLocationSelector"
        Layout.fillWidth: true
    }

    SpinBoxWithLabel {
        id: subRectXStart
        objectName: "subRectXStart"
        text: qsTr("Sub Rect X-start")
        value: 0
    }

    SpinBoxWithLabel {
        id: subRectYStart
        objectName: "subRectYStart"
        text: qsTr("Sub Rect Y-start")
        value: 0
    }

    SpinBoxWithLabel {
        id: subRectXEnd
        objectName: "subRectXEnd"
        text: qsTr("Sub Rect X-end")
        value: 0
    }

    SpinBoxWithLabel {
        id: subRectYEnd
        objectName: "subRectYEnd"
        text: qsTr("Sub Rect Y-end")
        value: 0
    }

    Button {
        id: btnSendSubRect
        objectName: "btnSendSubRect"
        text: qsTr("Publish Job")
        onClicked: uicore.doSendJobSubRect()
    }
}
