import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

import my.work.core 1.0
import my.work.gui 1.0

Item {
    width: 800
    height: 600


    Logger {
        id: logger
        values: devises.getValues(true)

//        filePath: "/home/alex13sh/TMP/test_2.sqlite"
        filePath: "./test_3.sqlite"
        isWrite: sessionPane.isStart
        isRead: !sessionPane.isStart
        Component.onCompleted: {
            var ses = this.sessions;
//            sessionsList.model = ses
        }
    }
    Timer {
        interval: 1000; repeat: true
        running: sessionPane.isStart
        property int macCnt: 20
        onTriggered: {
//            logger.readValues()
            chart.updateSensors()
            logger.pushValues()
//            macCnt--;
        }
    }

    ModbusDevicesInit {
        id: devises
    }

    GridLayout{
        anchors.fill: parent
        columns: 3
        rows: 2
        flow: GridLayout.TopToBottom

        SessionsList {
            id: sessionsList
//            visible: !pbStart.checked
            Layout.minimumWidth: !sessionPane.isStart ?200:0
//            Layout.maximumWidth: 200
            Layout.minimumHeight: 200
            Layout.fillHeight: true
            Layout.column: 0
            Layout.row: 0

            model: logger.sessions
            onSelected: {
                for (var i in chart.lstLS){
    //            var s = sensorList.model.at(0)
                    var s = chart.lstLS[i]
                    s.sers.visible = false
                    var hash = s.sens.hash
                    if (hash==="") continue;

//                    console.log("selected start:", start, ", finish:", finish, ", hash:", hash)
                    var lst = logger.getValuesPoint_var(hash, start, finish)
                    chart.setValuesPoints(s.sers, start, finish, lst)
                    chart.focus = true
                }
            }
        }

        SensorList {
            id: sensorList
            width: 150
            Layout.minimumWidth: 200
            Layout.maximumWidth: 250
    //        Layout.preferredWidth: parent.width*20
            Layout.minimumHeight: 200
            Layout.fillHeight: true
            Layout.column: 1
            Layout.row: 0
        }
        SessionPane {
            id: sessionPane
            Layout.column: 0
            Layout.row: 1
            Layout.columnSpan: 2

            Layout.fillWidth: true
            Layout.minimumHeight: 100
        }

        Graphics {
            id: chart
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.column: 2
            Layout.row: 0
        //  Layout.rowSpan: 2
        }

        SensorPane {
            Layout.fillWidth: true
            Layout.minimumHeight: 100
            sensor: chart.lstLS[sensorList.currentIndex].sens
        }
    }
}
