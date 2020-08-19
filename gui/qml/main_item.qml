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

//        filePath: "/home/alex13sh/TMP/test_2.sqlite"
        filePath: "./test_2.sqlite"
        isWrite: pbStart.checked
        isRead: !pbStart.checked
        Component.onCompleted: {
//            isRead = true

            var ses = this.sessions;

            for(var i in ses){
                var hashs = ses[i].getValuesHash()
//                console.log("Session start:", ses[i].start, "; finish:", ses[i].finish)
//                console.log("hashs:", hashs)
            }
//            sessionsList.model = ses
        }
    }
    Timer {
        interval: 500; repeat: true
        running: pbStart.checked
        property int macCnt: 20
        onTriggered: {
//            logger.readValues()
            chart.updateSensors()
//            macCnt--;
        }
    }

    ModbusDevicesInit {
        id: devises
//        onMapDevicesChanged: logger.values = devises.getValues(true)
//        Component.onCompleted: logger.values = devises.getValues(true)
    }

    GridLayout{
        anchors.fill: parent
        columns: 3
        rows: 2
        flow: GridLayout.TopToBottom

        SessionsList {
            id: sessionsList
//            visible: !pbStart.checked
            Layout.minimumWidth: !pbStart.checked ?200:0
//            Layout.maximumWidth: 200
            Layout.minimumHeight: 200
            Layout.fillHeight: true
            Layout.column: 0
            Layout.row: 0
            color: "red"
            clip: true
            model: logger.sessions
            onSelected: {
                for (var i in chart.lstLS){
    //            var s = sensorList.model.at(0)
                    var s = chart.lstLS[i]
                    s.sers.visible = false
                    var hash = s.sens.hash
                    if (hash==="") continue;

                    console.log("selected start:", start, ", finish:", finish, ", hash:", hash)
                    var lst = logger.getValuesPoint_var(hash, start, finish)
                    chart.setValuesPoints(s.sers, start, finish, lst)
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
        Item {
            Layout.column: 0
            Layout.row: 1
            Layout.columnSpan: 2

            Layout.fillWidth: true
            Layout.minimumHeight: 100

            Button {
                id: pbStart
                text: checked? "Stop" : "Start"
                checkable: true
                checked: false
            }
        }

        MyChartView {
            id: chart
            title: "Top-5 car brand shares in Finland"
    //        anchors.fill: parent
            legend.alignment: Qt.AlignRight
    //        Layout.preferredWidth: parent.width*80
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.column: 2
            Layout.row: 0
//            Layout.rowSpan: 2

            legend.visible: false
            focus: true

            axisTemer: axis_temper
            ValueAxis {
                id: axis_temper
                min: 0
                max: 100
                minorTickCount: 5
                tickCount: 10
            }
            ValueAxis {
                id: axis_other
                min: -10
                max: 200
                minorTickCount: 10
            }

            axisDavl: axis_davl
            LogValueAxis {
                id: axis_davl
                min: 0.001
                max: 1000
                minorTickCount: 10
            }

            axisDate: axis_dt
            DateTimeAxis{
                id: axis_dt
                tickCount: 10
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
                var lstSens = devises.getListSensors()

    //            for(var i in lst) {
    //                console.log(i, ") name:", lst[i].name)
    //                var ls = cmpLS.createObject(chart)
    //                ls.name = lst[i].name
    //                ls.sens = lst[i]
    //                lstLS.push(ls)
    //            }
                var lstLS_ = setSensors(lstSens)
                for (var i in lstLS_){
                    var ls = {
                        name: lstSens[i].name,
                        sens: lstSens[i],
                        sers: lstLS_[i]
                    }
                    lstLS.push(ls)
                }

                sensorList.model = lstLS
            }
        }

        Rectangle {
            color: "red"
            Layout.fillWidth: true
            Layout.minimumHeight: 100
            Text {
                text: "Current sensor: " + chart.lstLS[sensorList.currentIndex].sens.name
            }
        }
    }
}
