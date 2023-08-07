import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout
{
    id: logMessagesTab
    objectName: "logMessagesTab"

    Label {
        id: messagesLabel
        Layout.alignment: Qt.AlignTop
        text: "Log Messages:"
    }

    ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        background: Rectangle {
            border.color: "lightgray"
            border.width: 1
        }

        ListView
        {
            id: messageList
            focus: true
            clip: true
            keyNavigationWraps: false
            spacing: 1
            leftMargin: 2

            model: logger

            delegate: Text {
                required property string logMessageLine
                text: logMessageLine
                width: messageList.width
                MouseArea {
                    anchors.fill: parent
                    onClicked: list.currentIndex = index
                }
            }

            highlight: Rectangle {
                color: "lightsteelblue";
            }
        }
    }

    Button {
        id: btnClearLogMessages
        Layout.alignment: Qt.AlignBottom
        text: qsTr("Clear Log Messages")
        onClicked: logger.clear()
    }
}
