project "Tests"
    kind "ConsoleApp"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{wks.location}/Framework/source"
    }

    files {
        "./source/**.cpp",
        "./source/**.h"
    }

    libdirs {
    }

    links { "Framework" }