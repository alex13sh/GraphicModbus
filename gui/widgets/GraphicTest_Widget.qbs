CppApplication{
    condition: false
    Depends{name:"Qt"; submodules: ["gui","widgets", "charts"]}
    Depends{
        name: "GraphicTest_Core"
    }

//    cpp.includePaths: {
//        var lst = base.concat([product.sourceDirectory, qbs.installRoot+qbs.installPrefix+"/include"])
//        console.info("includePaths:"+lst);
//        return lst
//    }
    cpp.includePaths: [product.sourceDirectory]

    files: ["*.h", "*.cpp", "*.ui"]
    excludeFiles: ["*form.*", "*widget.*", "*window.*"]

    Group {
        name: "Forms"
        files: "*form.*"
    }
    Group {
        name: "Widgets"
        files: "*widget.*"
    }
    Group {
        name: "Window"
        files: "*window.*"
    }


    Group{
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "bin"
    }
}
