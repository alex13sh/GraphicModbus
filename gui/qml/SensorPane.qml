import QtQuick 2.0

Item {

    property var sensor: null
    Text {
        text: "Current sensor: " + sensor.name
    }
}
