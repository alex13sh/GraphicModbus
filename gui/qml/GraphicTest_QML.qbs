import qbs.FileInfo
import qbs.ModUtils
import qbs.TextFile

CppApplication{
    Depends{name:"cpp"}
    Depends{name:"Qt"; submodules: ["gui","qml", "quick"]}
    Depends{ name: "GraphicTest_Core" }
    Depends{name:"Qt"; submodules: ["charts", "charts-private", "widgets"]; condition: isViewer}

    Qt.qml.importName: "my.work.gui"
    Qt.qml.importVersion: "1.0"
    cpp.includePaths: "./"
//    cpp.staticLibraries: "C:/Qt/5.15.0/mingw81_64/qml/QtCharts/qtchartsqml2.dll"
    cpp.staticLibraries: qbs.targetOS.contains("unix")
            ? "/run/media/alex13sh/linux_opt/Qt/5.15.0/gcc_64/qml/QtCharts/libqtchartsqml2.so"
            : qbs.targetOS.contains("windows")
            ? qbs.installRoot+"/bin/qtchartsqml2.dll"
            : ""

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
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "bin"
    }
    Group {
        fileTagsFilter: "dynamiclibrary_import"
        filesAreTargets: true
        qbs.install: true
        qbs.installDir: "lib"
    }

    Rule {
        condition: false
        multiplex: true
//        auxiliaryInputs: true
        inputsFromDependencies: ["dynamiclibrary_import"]
        outputFileTags: "mylibrary"
//        Artifact {
//            filePath: product.name + ".tarlist"
//            fileTags: "mylibrary"
//        }
//        prepare: {
//            var cmd = new JavaScriptCommand();
//            cmd.silent = true;
//            cmd.sourceCode =function() {
//                var ofile = new TextFile(output.filePath, TextFile.WriteOnly);
//                try {
//                    for (var i = 0; i < inputs["dynamiclibrary_import"].length; ++i) {
//                        var inp = inputs["dynamiclibrary_import"][i];
//                        var installRoot = inp.moduleProperty("qbs", "installRoot");
//                        var installedFilePath = ModUtils.artifactInstalledFilePath(inp);
//                        ofile.writeLine(FileInfo.relativePath(installRoot, installedFilePath));
//                    }
//                } finally {
//                    ofile.close();
//                }
//            };
//            return [cmd];
//        }
    }
}
