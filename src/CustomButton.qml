import QtQuick 2.0
import QtQuick.Controls 2.3

ToolButton {
    id: buttonid
    text: qsTr("ToolButton")
    width: 120

    contentItem: Image {
        id: imageid
        source: undefined
        width: 20
        height: 20
        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(parent.width, parent.height)
    }

    background: Rectangle {
        anchors.fill: parent
        implicitWidth: 40
        implicitHeight: 40
        radius: 5
        border.color: "#CCCCCC"
        border.width: 1
        color: buttonid.enabled ? (buttonid.checked ? "#CCCCCC" : (buttonid.hovered ? "#F6F6F6" : "#FFFFFF")) : "#EFEFEF"
    }

    checked: false
    highlighted: false
}



