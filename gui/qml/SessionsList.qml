import QtQuick 2.0

Rectangle {
    id: root
    color: "grey"
    clip: true
    border{
        color: "black"
        width: 2
    }

    signal selected(int index, date start, date finish);

    property alias model: view.model
    ListView {
        id: view
        anchors.fill: parent
        spacing: 2
        delegate: Rectangle {
            id: delegate
            property date start: modelData.start
            property date finish: modelData.finish

            color: ListView.isCurrentItem ? "grey" : "white"
            width: view.width
            height: column.height
            Column{
                id: column
                Text {
                    width: delegate.width
                    text: "start: " + delegate.start.toLocaleString(Qt.locale(), "dd hh:mm:ss")
                    wrapMode: Text.WordWrap
                    font.pixelSize: 14
                }
                Text {
                    width: delegate.width
                    clip: true
                    text: "finish: "+delegate.finish.toLocaleString(Qt.locale(), "dd hh:mm:ss")
                    font.pixelSize: 14
                }
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    view.currentIndex = index
                    root.selected(delegate.index, start, finish)
                }
            }
        }
    }
}
