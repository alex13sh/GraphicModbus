import QtQuick 2.0
import QtQuick.Controls 2.0

import my.work.core 1.0
import my.work.gui 1.0

import QtCharts 2.0

Item {
    width: 800
    height: 600


    Logger {
        id: logger
        values: devises.getValues(true)
    }

    ModbusDevicesInit {
        id: devises
//        onMapDevicesChanged: logger.values = devises.getValues(true)
//        Component.onCompleted: logger.values = devises.getValues(true)
    }

    ChartView {
        id: chart
        title: "Top-5 car brand shares in Finland"
        anchors.fill: parent
        legend.alignment: Qt.AlignBottom

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

        Component{
            id: cmpLS
        LineSeries {
//            id: lineSeries1
            name: "signal 1"
//            axisX: axisX
//            axisY: axisY1
            useOpenGL: true
        }
        }

        Component.onCompleted: {
            var lst = devises.sensors

            for(var i in lst) {
                console.log(i, ") name:", lst[i].name)
                var ls = cmpLS.createObject(chart)
                ls.name = lst[i].name
            }
        }
    }
}
