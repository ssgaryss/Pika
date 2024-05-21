workspace "Pika"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include diretories to relative root folder
includeDir = {}
includeDir["GLFW"] = "Pika/vendor/GLFW/include"
includeDir["glad"] = "Pika/vendor/glad/include"

include "Pika/vendor/GLFW"
include "Pika/vendor/glad"

-- Pika
project "Pika"
	location "Pika"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader("pkpch.h")
	pchsource("Pika/src/pkpch.cpp")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PK_PLATFORM_WINDOWS",
			"PK_BUILD_DLL",
			"PK_ENABLE_ASSERTS",
			-- we don not want GLFW include OpenGL function, glad got all of it!
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PIKA_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "PIKA_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "PIKA_DIST"
		buildoptions "/MD"
		optimize "On"

-- Sandbox
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pika/vendor/spdlog/include",
		"Pika/src"
	}

	links
	{
		"Pika"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PIKA_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "PIKA_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "PIKA_DIST"
		buildoptions "/MD"
		optimize "On"
	