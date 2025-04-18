project "Framework"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "./source/pch.cpp"

    includedirs {
        "./source",
        "./vendor/glfw/include",
        "./vendor/spdlog/include",
        "./vendor/ImGui"
    }

    files {
        "./source/**.cpp",
        "./source/**.h"
    }

    libdirs {
    }

    links { "glfw", "ImGui" }
    
    filter "platforms:Win64"
        links { "d3d11", "dxguid" }