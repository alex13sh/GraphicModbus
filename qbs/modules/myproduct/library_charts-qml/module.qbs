
Module {
    id: module
    condition: true
    Depends { name: "cpp" }
    Depends { name: "Qt.qml" }
    // cpp.staticLibraries: "C:/Qt/5.15.0/mingw81_64/qml/QtCharts/qtchartsqml2.dll"
    cpp.staticLibraries: libPath + libName
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
        name: "Static Library"
        files: Qt.qml.qmlPath + "/QtCharts/" + module.libName
        fileTags: ["dynamiclibrary", "dynamiclibrary_import"]
        filesAreTargets: true
//        qbs.install: true
//        qbs.installDir: module.libDir
    }
}
