import QtQuick 2.0
import QtQuick.Controls 2.0

Item {

    property var sensor: null
    Column {
        spacing: 5
        Text {
            text: "Current sensor: " + sensor.name
        }
        Text {
            text: "Value Warn: " + sensor.value_warn + "; Err: " + sensor.value_err
        }
        Button {
            visible: sensor.logicLevel ? true : false
            checkable: true
            checked: sensor.logicLevel ? sensor.logicLevel() : false
            text: checked ? "Выключить" : "Включить"
            onCheckedChanged: {
                if ( sensor.logicLevel) {
                    sensor.setLogicLevel (checked)
                }
            }
        }
    }
}
