Project {
    qbsSearchPaths: "./qbs"
    references: [
        "./core/GraphicTest_Core.qbs",
        "./gui/widgets/GraphicTest_Widget.qbs",
        "./gui/qml/GraphicTest_QML.qbs",
    ]

//    WindowsInstallerPackage{

//    }
    InstallPackage {
        archiver.type: "tar"
        name: "tar-package"
        Depends {name:"GraphicTest_QML"}
//        Depends {name:"GraphicTest_Core"}
    }
}
