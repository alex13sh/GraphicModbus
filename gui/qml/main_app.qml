import QtQuick 2.0
import QtQuick.Controls 2.0

import my.work.core 1.0
import my.work.gui 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Graphic QML"


    Logger {
        id: logger
        values: devises.getValues(true)
    }

    ModbusDevicesInit {
        id: devises
//        onMapDevicesChanged: logger.values = devises.getValues(true)
//        Component.onCompleted: logger.values = devises.getValues(true)
    }

}
