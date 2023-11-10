workspace "Zital"
	architecture "x64"

	configurations
	{
		"Debug", 
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "Zital/Vendor/GLFW/include"
IncludeDir["Glad"] = "Zital/Vendor/Glad/include"
IncludeDir["ImGui"] = "Zital/Vendor/imgui"
IncludeDir["glm"] = "Zital/Vendor/glm"

include "Zital/Vendor/GLFW"
include "Zital/Vendor/Glad"
include "Zital/Vendor/imgui"

project "Zital"
	location "Zital"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ZTpch.h"
	pchsource "Zital/Source/ZTpch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/glm/**.hpp",
		"%{prj.name}/Vendor/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ZT_PLATFORM_WINDOWS",
			"ZT_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_WINDLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ZT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ZT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ZT_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Zital/Vendor/spdlog/include",
		"Zital/Source",
		"Zital/Vendor",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"Zital"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ZT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ZT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ZT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ZT_DIST"
		runtime "Release"
		optimize "On"