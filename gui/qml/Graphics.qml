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

    property alias axis_temper: axis_temper
    axisTemer: axis_temper
    ValueAxis {
        id: axis_temper
        min: 0
        max: 100
        minorTickCount: 5
        tickCount: 10
        visible: false
    }
    axisVibra: axis_other
    ValueAxis {
        id: axis_other
        min: -1
        max: 5
//        minorTickCount: 10
        visible: true
        gridVisible: false
        minorGridVisible: false
    }

    property alias axis_davl: axis_davl
    axisDavl: axis_davl
    LogValueAxis {
        id: axis_davl
        min: 0.001
        max: 1000
        minorTickCount: 5
        visible: false
        gridVisible: true
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

    MouseArea {
        anchors.fill: parent
        onClicked: parent.focus = true
    }
}
