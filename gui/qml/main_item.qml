import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

import my.work.core 1.0
import my.work.gui 1.0

import QtCharts 2.0

Item {
    width: 800
    height: 600


    Logger {
        id: logger
        values: devises.getValues(true)

        filePath: "/home/alex13sh/TMP/test_2.sqlite"
//        filePath: "./test_2.sqlite"
        Component.onCompleted: {
//            isRead = true

            var ses = this.sessions;

            for(var i in ses){
                var hashs = ses[i].getValuesHash()
//                console.log("Session start:", ses[i].start, "; finish:", ses[i].finish)
//                console.log("hashs:", hashs)
            }
            sessionsList.model = ses
        }
    }
    Timer {
        interval: 20; repeat: true
        running: true
        onTriggered: logger.readValues()
    }

    ModbusDevicesInit {
        id: devises
//        onMapDevicesChanged: logger.values = devises.getValues(true)
//        Component.onCompleted: logger.values = devises.getValues(true)
    }

    GridLayout{
        anchors.fill: parent
        columns: 2
        rows: 2

    SensorList {
        id: sensorList
        width: 150
        Layout.minimumWidth: 200
        Layout.maximumWidth: 250
//        Layout.preferredWidth: parent.width*20
        Layout.fillHeight: true
    }

    ChartView {
        id: chart
        title: "Top-5 car brand shares in Finland"
//        anchors.fill: parent
        legend.alignment: Qt.AlignRight
//        Layout.preferredWidth: parent.width*80
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.rowSpan: 2

        ValueAxis {
            id: axis_temper
            min: 0
            max: 100
            minorTickCount: 10
        }
        ValueAxis {
            id: axis_other
            min: -10
            max: 200
            minorTickCount: 10
        }

        LogValueAxis {
            id: axis_davl
            min: 0.001
            max: 1000
            minorTickCount: 10
        }
        DateTimeAxis{
            id: axis_dt

            tickCount: 20
        }

        Component{
            id: cmpLS
            LineSeries {
    //            id: lineSeries1
                name: "signal 1"
                axisX: axis_dt
                axisY: axis_temper
//                useOpenGL: true
                property var sens
            }
        }

        property var lstLS : []
        Component.onCompleted: {
            var lst = devises.getListSensors()


            for(var i in lst) {
                console.log(i, ") name:", lst[i].name)
                var ls = cmpLS.createObject(chart)
                ls.name = lst[i].name
                ls.sens = lst[i]
                lstLS.push(ls)
            }
            sensorList.model = lstLS
        }
    }
    SessionsList {
        id: sessionsList
        Layout.minimumWidth: 200
        Layout.minimumHeight: 200
//        Layout.fillHeight: true
//        Layout.column: 0
        color: "red"
        clip: true
        onSelected: {
//            var s = sensorList.model.at(0)
            var s = chart.lstLS[0]
            var hash = "be7a63150f" //logger.sessions[index].getValuesHash()[0]
            console.log("selected start:", start, ", finish:", finish, ", hash:", hash)
            var lst = logger.getValuesPoint_var(hash, start, finish)
            devises.series_setPoints(hash, lst, s)
            s.visible = true
        }
    }
    }
}
