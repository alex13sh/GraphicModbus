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
        filePath: "./test_4.sqlite"
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
            devises.updateValues()
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
                    if (hash==="") {
                        console.log("Empty hash sensor name:", s.sens.name)
                        continue;
                    }
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

            isStart: sessionPane.isStart
            onCurrentIndexChanged: {
                var sens = chart.lstLS[sensorList.currentIndex].sens
                if (sens.hash === "86c6deedfb") {
                    chart.axis_temper.visible = false
                    chart.axis_temper.gridVisible = false
                    chart.axis_davl.gridVisible = true
                    chart.axis_davl.visible = true
                } else {
                    chart.axis_temper.visible = true
                    chart.axis_temper.gridVisible = true
                    chart.axis_davl.gridVisible = false
                    chart.axis_davl.visible = false
                }
                console.log("sens hash:", sens.hash)
            }
        }
        SessionPane {
            id: sessionPane
            Layout.column: 0
            Layout.row: 1
            Layout.columnSpan: 2

            Layout.fillWidth: true
            Layout.minimumHeight: 100

            txt_start: logger.start
            onIsStartChanged: {
                if(isStart) {
                    chart.clearValues()
                    txt_start = logger.start
                }
            }
        }

        Graphics {
            id: chart
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.column: 2
            Layout.row: 0
        //  Layout.rowSpan: 2

            Component {
                id: cmpLS
                QtObject {
                    property string name: ""
                    property var sens
                    property var sers
                    property double value: -1
                }
            }

            property var lstLS : []
            Component.onCompleted: {
                var lstSens = devises.getListSensors()

                var lstLS_ = setSensors(lstSens)
                for (var i in lstLS_){
                    var ls = {
                        name: lstSens[i].name,
                        sens: lstSens[i],
                        sers: lstLS_[i],
                        value: -1
                    }
                    lstLS.push(cmpLS.createObject(this, ls))
                }

                sensorList.model = lstLS
            }

            onSelectedValues: {
                sessionPane.txt_curTime = time.toLocaleString(Qt.locale(), "hh:mm:ss.zzz")
                for (var i in lstLS)
                    lstLS[i].value = values[i]
            }
        }

        SensorPane {
            Layout.fillWidth: true
            Layout.minimumHeight: 100
            sensor: chart.lstLS[sensorList.currentIndex].sens
        }
    }
}
