include "/Vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Zital"
	architecture "x86_64"
	startproject "Zital-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.arcitecture}"

group "Dependencies"
	include "Vendor/premake" 
	include "Zital/Vendor/GLFW"
	include "Zital/Vendor/Glad"
	include "Zital/Vendor/imgui"
	include "Zital/Vendor/yaml-cpp"
group ""

include "Zital"
include "Sandbox"
include "Zital-Editor"