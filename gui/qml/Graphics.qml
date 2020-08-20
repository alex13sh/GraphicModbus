import QtQuick 2.0
import my.work.gui 1.0
import QtCharts 2.0

MyChartView {
    id: chart
    title: "Top-5 car brand shares in Finland"
//        anchors.fill: parent
    legend.alignment: Qt.AlignRight
//        Layout.preferredWidth: parent.width*80


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
