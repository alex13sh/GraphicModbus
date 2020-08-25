import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property alias isStart: pbStart.checked
    property string txt_start: ""
    property string txt_curTime: ""

    signal openJsonFile(string fileName);

    Row {
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
        TextField {
            id: txtFileName
            visible: !isStart
            placeholderText: "FileName"
            onEditingFinished: {
                if (text.endsWith(".json")) {
                    console.log("FileOpen: ", text)

                }
            }
        }
    }
}
