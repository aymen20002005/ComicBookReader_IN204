import QtQuick 2.6
import QtQuick.Controls 2.1


Slider {
    id: sliderid

    background: Rectangle {
        x: sliderid.leftPadding
        y: sliderid.topPadding + sliderid.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 6
        width: sliderid.availableWidth
        height: implicitHeight
        radius: 3
        color: "#E6E6E6"

        Rectangle {
            color : "#1545DA"
            radius: 3
            width: sliderid.visualPosition * parent.width
            height: parent.height

        }
    }

    handle: Rectangle {
        x: sliderid.leftPadding + sliderid.visualPosition * (sliderid.availableWidth - width)
        y: sliderid.topPadding + sliderid.availableHeight / 2 - height / 2
        implicitWidth: 16
        implicitHeight: 16
        radius: 8
        color: "#F6F6F6"
        border.color: "#D9D9D9"
        border.width: 1
    }
    ToolTip {
        opacity: 0.8
        parent: sliderid.handle
        visible: sliderid.pressed
        text: sliderid.value
    }

}
