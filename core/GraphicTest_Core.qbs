 DynamicLibrary{
//StaticLibrary{
    id: lib
//    name: ""
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["serialport", "serialbus", "sql", "qml"]}

    files: ["*.h", "*.cpp"]
    Qt.core.generateMetaTypesFile: true
    Qt.qml.importName: "my.work.core"
    Qt.qml.importVersion: "1.0"

    cpp.includePaths:"./"

    cpp.defines: [
//        "LOGGER_SQL",
        "LOGGER_JSON",
    ]

    Export{
//        property stringList qmlImportPaths: [qbs.installRoot + "/lib/"]
        Depends{name:"cpp"}
        Depends{name:"Qt"; submodules: ["qml", "sql", "serialbus"]}
//        cpp.includePaths:"./"
//        cpp.includePaths: [qbs.installRoot+qbs.installPrefix+"/include"]
        cpp.includePaths: [product.sourceDirectory+"/../"]
    }

//    Qt.core.pluginPath: "plugins"
//    Qt.core.pluginMetaData: ["uri="+Qt.qml.importName]
//    property bool qbs_install: false
//    installImportLib: true
    install: true
//    Group{
//        fileTagsFilter: product.type
//        qbs.install: true
//        qbs.installDir: "lib"
//    }
    Group {
        fileTagsFilter: "hpp"
        qbs.install: true
        qbs.installDir: "include/core"
    }
}
