project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Zital/Vendor/spdlog/include",
        "%{wks.locaiton}/Zital/Source",
        "%{wks.location}/Zital/Vendor",
        "%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "Zital"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ZT_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ZT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ZT_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ZT_DIST"
        runtime "Release"
        optimize "on"