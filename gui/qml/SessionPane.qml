import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property alias isStart: pbStart.checked
    Button {
        id: pbStart
        text: checked? "Stop" : "Start"
        checkable: true
        checked: false
    }
}
