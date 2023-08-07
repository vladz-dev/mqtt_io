import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {

    property alias text: labelText.text
    property alias value: spinValue.value

    Label {
        id: labelText
        Layout.alignment: Qt.AlignVCenter
    }

    SpinBox {
        id: spinValue
        Layout.alignment: Qt.AlignVCenter
    }
}
