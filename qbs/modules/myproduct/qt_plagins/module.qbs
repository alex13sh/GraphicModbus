
Module {
    id: module
    condition: qbs.targetOS.contains("windows")
    Depends { name: "Qt.core" }

    property string libName: qbs.targetOS.contains("unix")
             ? "libqtchartsqml2.so"
             : qbs.targetOS.contains("windows")
             ? "qtchartsqml2.dll"
             : ""
    property string libDir: qbs.targetOS.contains("unix")
            ? "lib"
            : qbs.targetOS.contains("windows")
            ? "bin"
            : ""
    property string libPath: qbs.installRoot + qbs.installPrefix +"/"+ libDir +"/"
    Group {
        name: "Platforms"
//        prefix: Qt.core.pluginPath
        files: Qt.core.pluginPath+"/platforms/*"
        filesAreTargets: true
        qbs.installSourceBase: Qt.core.pluginPath
        qbs.install: true
        qbs.installDir: module.libDir
    }
    Group {
        name: "SqlDrivers"
//        prefix: Qt.core.pluginPath
        files: Qt.core.pluginPath+"/sqldrivers/*"
        filesAreTargets: true
        qbs.installSourceBase: Qt.core.pluginPath
        qbs.install: true
        qbs.installDir: module.libDir
    }
}
