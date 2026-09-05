include "Dependencies.lua"

workspace "Pika"
	architecture "x64"

	filter "system:macosx"
		architecture "ARM64" -- native Apple Silicon; use "Universal" for a fat binary

	filter {}
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
	include "Pika/vendor/assimp"
group ""

-- macOS: the vendored zlib bundled with assimp calls read/write/close without
-- including <unistd.h>, and clang 15+ treats implicit function declarations as
-- errors. Downgrade that back to a warning; the implicit signatures of
-- close/read/write are compatible for zlib's buffer-sized use.
project "assimp"
	filter "system:macosx"
		buildoptions { "-Wno-implicit-function-declaration" }

group "Core"
	include "Pika"
	include "Pikartisan"
group ""

group "Misc"
	include "Sandbox"
group ""