project "Framework"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "./source/pch.cpp"

    includedirs {
        "./vendor/glfw/include"
    }

    files {
        "./source/**.cpp",
        "./source/**.h"
    }

    libdirs {
    }

    links { "glfw" }