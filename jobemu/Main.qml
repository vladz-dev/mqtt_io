import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts


Window {
    id: root
    width: 640
    height: 300
    visible: true
    title: qsTr("Job Emulator")

    Component.onCompleted:
    {
        btnConnect.enabled = true
        btnDisconnect.enabled = false
        logMessagesTab.enabled = true

        tabButtonJobMirror.enabled = false
        tabButtonJobRgb2Gbr.enabled = false
        tabButtonJobSubRect.enabled = false

        jobMirrorTab.enabled = false
        jobRgb2GbrTab.enabled = false
        jobSubRectTab.enabled = false
    }

    Connections {
        target: emulator
        function onConnected() {
            console.info("Connected")
            uicore.refreshControls();
        }
        function onDisconnected() {
            console.info("Disconnected")
            uicore.refreshControls();
        }
    }

    ColumnLayout
    {
        anchors.fill: parent;
        spacing: 10

        RowLayout
        {
            id: jobTabs
            Layout.alignment: Qt.AlignVCenter | Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: false

            TabBar {
                id: jobsBar
                objectName: "jobsBar"
                Layout.fillWidth: false

                TabButton {
                    id: tabButtonJobMirror
                    objectName: "tabButtonJobMirror"
                    text: qsTr("Job Mirror")
                    width: implicitWidth
                    background: Rectangle {
                        color: jobsBar.currentIndex === 0 ? "orange" : "lightgray"
                    }
                }
                TabButton {
                    id: tabButtonJobRgb2Gbr
                    objectName: "tabButtonJobRgb2Gbr"
                    text: qsTr("Job RGB-GBR")
                    width: implicitWidth
                    background: Rectangle {
                        color: jobsBar.currentIndex === 1 ? "orange" : "lightgray"
                    }
                }
                TabButton {
                    id: tabButtonJobSubRect
                    objectName: "tabButtonJobSubRect"
                    text: qsTr("Job SubRect")
                    width: implicitWidth
                    background: Rectangle {
                        color: jobsBar.currentIndex === 2 ? "orange" : "lightgray"
                    }
                }
                TabButton {
                    text: qsTr("Log Messages")
                    width: implicitWidth
                    background: Rectangle {
                        color: jobsBar.currentIndex === 3 ? "orange" : "lightblue"
                    }
                }
            }

            Button {
                id: btnConnect

                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                objectName: "btnConnect"
                text: qsTr("Connect")
                onClicked: emulator.doConnect()
            }

            Button {
                id: btnDisconnect

                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                objectName: "btnDisconnect"
                text: qsTr("Disconnect")
                onClicked: emulator.doDisconnect()
            }
        }

        StackLayout {
            id: jobsArgs
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            currentIndex: jobsBar.currentIndex

            JobImageMirror {
                id: jobMirrorTab
                objectName: "jobMirrorTab"
            }

            JobImageRgb2Gbr {
                id: jobRgb2GbrTab
                objectName: "jobRgb2GbrTab"
            }

            JobImageSubRect {
                id: jobSubRectTab
                objectName: "jobSubRectTab"
            }

            LogMessages {
                id: logMessagesTab
                objectName: "logMessagesTab"
            }
        }

    }
}
