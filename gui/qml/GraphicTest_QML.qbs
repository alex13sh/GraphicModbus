
CppApplication{
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["gui","qml", "quick"]}
    Depends{ name: "GraphicTest_Core" }
    Depends{name:"Qt"; submodules: ["charts", "charts-private", "widgets"]; condition: isViewer}

    Qt.qml.importName: "my.work.gui"
    Qt.qml.importVersion: "1.0"
    cpp.includePaths: "./"
    Depends {name: "myproduct.library_charts-qml"}
    Depends {name: "myproduct.library_qt"}

    property bool isViewer: true

    files: ["*.h", "*.cpp"]
    excludeFiles: isViewer?"main_app.cpp":"main_view.cpp"

    install: true

    Group {
        name: "QML"
        files: "*.qml"
        qbs.install: true
        qbs.installDir: "qml"
    }
//    Group {
//        fileTagsFilter: product.type
//        qbs.install: true
//        qbs.installDir: "bin"
//    }

    Export {
        Depends{ name: "GraphicTest_Core" }
    }

}
