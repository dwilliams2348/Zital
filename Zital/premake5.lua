project "Zital"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ZTpch.h"
    pchsource "Source/ZTpch.cpp"

    files
    {
        "Source/**.h",
        "Source/**.cpp",
        "Vendor/stb_image/**.h",
        "Vendor/stb_image/**.cpp",
        "Vendor/glm/glm/**.hpp",
        "Vendor/glm/glm/**.inl",

        "Vendor/ImGuizmo/ImGuizmo.h",
        "Vendor/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "Source",
        "Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:Vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }

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

        links
        {
            "%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
        }

    filter "configurations:Release"
        defines "ZT_RELEASE"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release",
			"%{Library.SPIRV_Cross_GLSL_Release}"
        }
    
    filter "configurations:Dist"
        defines "ZT_DIST"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release",
			"%{Library.SPIRV_Cross_GLSL_Release}"
        }