import qbs.File
import qbs.FileInfo
import qbs.ModUtils
import qbs.TextFile

Module {
    id: module
    condition: true
    additionalProductTypes: "library-qt"

    Depends { name: "Qt.core" }

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
            filePath: product.myproduct.library_qt.libPath + "lib_list.txt"
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
                        var baseName = inp.baseName
                        if (baseName.startsWith("lib")) baseName = baseName.slice(3)
                        ofile.writeLine(baseName);
                        if (inp.qbs.targetOS.contains("windows")
                            && baseName.startsWith("Qt"))
                            File.copy(inp.Qt.core.binPath +"/"+ baseName+".dll", libPath+baseName+".dll")
                        else File.copy(inp.filePath, libPath+inp.fileName)
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
