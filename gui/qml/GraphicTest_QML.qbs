
CppApplication{
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["gui","qml", "quick"]}
    Depends{ name: "GraphicTest_Core" }
    Depends{name:"Qt"; submodules: ["charts", "charts-private", "widgets"]; condition: isViewer}

    Qt.qml.importName: "my.work.gui"
    Qt.qml.importVersion: "1.0"
    cpp.includePaths: "./"
    cpp.staticLibraries: "/run/media/alex13sh/linux_opt/Qt/5.15.0/gcc_64/qml/QtCharts/libqtchartsqml2.so"

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
