include "Dependencies.lua"

workspace "Pika"
	architecture "x64"
	startproject "Pikartisan"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Pika/vendor/GLFW"
	include "Pika/vendor/glad"
	include "Pika/vendor/imgui"
	include "Pika/vendor/yaml-cpp"
	include "Pika/vendor/ImGuizmo"
	include "Pika/vendor/Box2D"
group ""

group "Core"
	include "Pika"
	include "Pikartisan"
group ""

group "Misc"
	include "Sandbox"
group ""