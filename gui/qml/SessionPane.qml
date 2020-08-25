import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property alias isStart: pbStart.checked
    property string txt_start: ""
    property string txt_curTime: ""

    signal openJsonFile(string fileName);

    Row {
        x: 20
        Column {
            spacing: 10
            Button {
                id: pbStart
                text: checked? "Stop" : "Start"
                checkable: true
                checked: false
            }
            Text {
                text: "Start: " + txt_start
                font.pixelSize: 14
            }
            Text {
                text: "Time:  " + txt_curTime
                font.pixelSize: 14
            }
        }
        TextField {
            id: txtFileName
            visible:  false // !isStart
            placeholderText: "FileName"
            onEditingFinished: {
                if (text.endsWith(".json")) {
                    console.log("FileOpen: ", text)

                }
            }
        }
    }
}
