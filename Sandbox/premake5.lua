project "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{wks.location}/Framework/source",
        "%{wks.location}/Framework/vendor/ImGui"
    }

    files {
        "./source/**.cpp",
        "./source/**.h"
    }

    libdirs {
    }

    links { "Framework" }