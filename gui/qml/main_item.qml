import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

import my.work.core 1.0
import my.work.gui 1.0

Item {
    width: 1280
    height: 940


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
        rows: 3
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
            Layout.rowSpan: 2

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
            Layout.minimumHeight: 210
            Layout.fillHeight: true
            Layout.column: 1
            Layout.row: 0

            isStart: sessionPane.isStart
            onCurrentIndexChanged: {
                var sens = chart.lstLS[sensorList.currentIndex].sens
                if (sens.hash === "86c6deedfb") {
                    chart.axis_temper.visible = false
                    chart.axis_davl.visible = true
                } else {
                    chart.axis_temper.visible = true
                    chart.axis_davl.visible = false
                }
            }
        }
        Column {
            Layout.minimumWidth: 200
            spacing: 10
            Button {
                text: "Уменьшить давление"
                checkable: true
                checked: false
                onClicked: {
//                    var klap_1 = chart.getSensor("308e553d36")
                    var klap_2 = chart.getSensor("615b2468bd")
                    var klap_3 = chart.getSensor("5b4afc628e")

                    klap_2.setLogicLevel(checked)
                    klap_3.setLogicLevel(checked)
                }
            }
            Button {
                text: "Увеличить давление"
                onClicked: {
                    var klap_1 = chart.getSensor("308e553d36")
//                    var klap_2 = chart.getSensor("615b2468bd")
//                    var klap_3 = chart.getSensor("5b4afc628e")
                    klap_1.setLogicLevel(checked)
                }
            }
        }

        SessionPane {
            id: sessionPane
            Layout.column: 0
            Layout.row: 2
            Layout.columnSpan: 2

            Layout.fillWidth: true
            Layout.minimumHeight: 150

            onIsStartChanged: {
                if(isStart) {
                    chart.clearValues()
                    txt_start = logger.start.toLocaleString(Qt.locale(), "hh:mm:ss.zzz")
                } else {
                    txt_curTime = logger.finish.toLocaleString(Qt.locale(), "hh:mm:ss.zzz")
                }
            }
        }

        Graphics {
            id: chart
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.column: 2
            Layout.row: 0
            Layout.rowSpan: 2

            isStart: sessionPane.isStart
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
            function getSensor(hash){
                for(var i in lstLS) {
                    if (lstLS[i].sens.hash === hash)
                        return lstLS[i].sens
//                    return null
                }
            }

            onSelectedValues: {
                sessionPane.txt_curTime = time.toLocaleString(Qt.locale(), "hh:mm:ss.zzz")
                for (var i in lstLS)
                    lstLS[i].value = values[i]
            }
        }

        SensorPane {
            Layout.fillWidth: true
            Layout.minimumHeight: 150
            sensor: chart.lstLS[sensorList.currentIndex].sens
        }
    }
}
