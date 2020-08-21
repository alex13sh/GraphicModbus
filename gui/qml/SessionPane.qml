import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property alias isStart: pbStart.checked
    property string txt_start: ""
    Column {
    Button {
        id: pbStart
        text: checked? "Stop" : "Start"
        checkable: true
        checked: false
    }
    Text {
        text: "Start:" + txt_start
    }
    }

}
