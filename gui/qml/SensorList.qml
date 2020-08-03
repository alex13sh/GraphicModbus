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
            border{
                width: 4
                color: modelData.color
            }
            width: view.width
            height: column.height
            Column{
                id: column
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
        }
    }
}
