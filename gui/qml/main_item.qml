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

        PieSeries {
            id: pieSeries
            PieSlice { label: "Volkswagen"; value: 13.5 }
            PieSlice { label: "Toyota"; value: 10.9 }
            PieSlice { label: "Ford"; value: 8.6 }
            PieSlice { label: "Skoda"; value: 8.2 }
            PieSlice { label: "Volvo"; value: 6.8 }
        }
    }
}
