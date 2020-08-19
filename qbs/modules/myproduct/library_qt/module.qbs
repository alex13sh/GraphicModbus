import qbs.File
import qbs.FileInfo
import qbs.ModUtils
import qbs.TextFile

Module {
    id: module
    condition: true
    additionalProductTypes: "library-qt"

    property string libDir: qbs.targetOS.contains("unix")
            ? "lib"
            : qbs.targetOS.contains("windows")
            ? "bin"
            : ""
    property string libPath: qbs.installRoot + "/" + qbs.installPrefix + "/" + libDir + "/"

    Rule {
        condition: true
        alwaysRun: true
        multiplex: true
        inputsFromDependencies: [/*"installable",*/ "dynamiclibrary", "dynamiclibrary_import"]
        Artifact {
            filePath: "~/TMP/test_list.lst"; // product.filePath + ".tarlist_2"
            fileTags: "library-qt"
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.silent = true;
            cmd.description = "copy library Qt"
            cmd.sourceCode =function() {
                var ofile = new TextFile(output.filePath, TextFile.WriteOnly);
                try {
                    for (var j in inputs){
//                        j = "dynamiclibrary"
                        ofile.writeLine("["+j+"]");
                    for (var i = 0; i < inputs[j].length; ++i) {
//                        console.info("infi i:", i)
                        var inp = inputs[j][i];
                        var libPath = inp.moduleProperty("myproduct.library_qt", "libPath");
//                        var libFilePathRelease = inp.moduleProperty("", "libFilePathRelease");
//                        var installedFilePath = ModUtils.artifactInstalledFilePath(inp);
//                        if (!installedFilePath) installedFilePath = installRoot + "/qt_lib"
//                        ofile.writeLine(FileInfo.relativePath(installRoot, installedFilePath));
                        ofile.writeLine(inp.filePath);
                        File.copy(inp.filePath, libPath+inp.fileName)
                    }
                    }
                } finally {
                    ofile.close();
                }
            };
            return [cmd];
        }
    }
}
