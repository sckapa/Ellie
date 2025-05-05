workspace "Sandbox"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    include "../../../../Ellie-ScriptCore"

    project "Sandbox"
        kind "SharedLib"
        language "C#"
        dotnetframework "4.7.2"

        targetdir ("Binaries")
        objdir ("Intermediates")

        files {
            "Source/**.cs",
            "Properties/**.cs"
        }

        links { "Ellie-ScriptCore" }
