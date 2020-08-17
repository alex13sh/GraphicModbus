import QtQuick 2.0

Rectangle {
    color: "grey"
    property alias model: view.model
    ListView {
        id: view
        anchors.fill: parent
        width: 100
        spacing: 2
        delegate: Rectangle {
            id: delegate
            property string name: modelData.name
            property double value: modelData.sens.value
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
                }
                Text {
                    width: delegate.width
                    clip: true
                    text: "value: "+delegate.value
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    delegate.sers.visible = !delegate.sers.visible
                }
            }
        }
    }
}
