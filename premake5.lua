workspace "Framework"
    configurations { "Debug", "Release", "Dist" }
    platforms { "Win64" }
    startproject "Framework"

    flags { "MultiProcessorCompile" }

    outputdir = "%{cfg.platform}-%{cfg.buildcfg}"

    systemversion "latest"

    filter "configurations:Debug" 
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

    filter "platforms:Win64"
        cppdialect "C++20"
        architecture "x86_64"
        buildoptions { "/ZI" }

    group "Dependencies"
        include "Framework/vendor/glfw"
        include "Framework/vendor/imgui"

    group "Core"
        include "Framework"

    group "Apps"
        include "Sandbox"
        include "Tests"


workspace "*"
    newaction {
        trigger     = "clean",
        description = "Remove binaries, intermediate files and vs project files",
        execute     = function ()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate files")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
        end
    }