import QtQuick 2.0

Rectangle {
    color: "grey"
    border{
        color: "black"
        width: 2
    }

    property alias model: view.model
    ListView {
        id: view
        anchors.fill: parent
        spacing: 2
        delegate: Rectangle {
            id: delegate
            property date start: modelData.start
            property date finish: modelData.finish

            width: view.width
            height: column.height
            Column{
                id: column
                Text {
                    width: delegate.width
                    text: "start: " + delegate.start
                    wrapMode: Text.WordWrap
                }
                Text {
                    width: delegate.width
                    clip: true
                    text: "finish: "+delegate.finish
                }
            }
        }
    }
}
