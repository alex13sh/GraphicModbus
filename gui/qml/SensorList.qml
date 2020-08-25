import QtQuick 2.0

Rectangle {
    color: "grey"
    property alias model: view.model
    property alias currentIndex: view.currentIndex
    property bool isStart: false
    ListView {
        id: view
        anchors.fill: parent
        width: 100
        spacing: 2
        delegate: Rectangle {
            id: delegate
            property string name: modelData.name
            property double value: {
                var v = isStart
                    ? modelData.sens.value
                    : modelData.value
                return v.toFixed(3)
            }
            property var sers: modelData.sers
            border{
                width: 4
                color: delegate.sers.visible ? delegate.sers.color : "grey"
            }
            width: view.width+5
            height: column.height+10
            Column{
                id: column
                x: 5; y: 5
                Text {
                    width: delegate.width
                    text: delegate.name
                    wrapMode: Text.WordWrap
                    font.pixelSize: 15
                }
                Text {
                    width: delegate.width
                    clip: true
                    text: "value: "+delegate.value
                    font.pixelSize: 15
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        delegate.sers.visible = !delegate.sers.visible
                    } else {
                        view.currentIndex = index
                    }
                }
            }
        }
    }
}
