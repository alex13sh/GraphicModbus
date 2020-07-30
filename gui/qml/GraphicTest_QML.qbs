
CppApplication{
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["gui","qml", "quick"]}
    Depends{ name: "GraphicTest_Core" }
    Depends{name:"Qt"; submodules: [/*"charts",*/ "widgets"]; condition: isViewer}

    Qt.qml.importName: "my.work.gui"
    Qt.qml.importVersion: "1.0"
    cpp.includePaths: "./"

    property bool isViewer: true

    files: ["*.h", "*.cpp"]
    excludeFiles: isViewer?"main_app.cpp":"main_view.cpp"

    Group {
        name: "QML"
        files: "*.qml"
        qbs.install: true
        qbs.installDir: "qml"
    }
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "bin"
    }

//        Qt.qml.typesInstallDir: "qml-type"
//        Group{
//            fileTagsFilter: "dynamiclibrary"
//            qbs.install: true
//            qbs.installDir: "lib"
//        }
}
