workspace "Zital"
	architecture "x64"

	configurations
	{
		"Debug", 
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	startproject "Zital-Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "Zital/Vendor/GLFW/include"
IncludeDir["Glad"] = "Zital/Vendor/Glad/include"
IncludeDir["ImGui"] = "Zital/Vendor/imgui"
IncludeDir["glm"] = "Zital/Vendor/glm"
IncludeDir["stb_image"] = "Zital/Vendor/stb_image"

group "Dependencies"
	include "Zital/Vendor/GLFW"
	include "Zital/Vendor/Glad"
	include "Zital/Vendor/imgui"

group ""

project "Zital"
	location "Zital"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ZTpch.h"
	pchsource "Zital/Source/ZTpch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Vendor/glm/**.hpp",
		"%{prj.name}/Vendor/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"ZT_ENABLE_ASSERTS"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ZT_PLATFORM_WINDOWS",
			"ZT_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_WINDLL"
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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


project "Zital-Editor"
	location "Zital-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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