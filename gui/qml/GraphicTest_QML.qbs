
CppApplication{
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["gui","qml"]}
    Depends{
        name: "GraphicTest_Core"
//            qbs_install: true
    }

    files: ["*.h", "*.cpp"]

    Group{
        name: "QML"
        files: "*.qml"
        qbs.install: true
        qbs.installDir: "qml"
    }
    Group{
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
