workspace "Pika"
	architecture "x64"
	startproject "Sandbox"
	
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
includeDir["ImGui"] = "Pika/vendor/imgui"
includeDir["glm"] = "Pika/vendor/glm"

group "Dependencies"
	include "Pika/vendor/GLFW"
	include "Pika/vendor/glad"
	include "Pika/vendor/imgui"
group ""

-- Pika
project "Pika"
	location "Pika"
	kind "SharedLib"
	language "C++"
	staticruntime "off" -- buildoptions "/MDd" --> [staticruntime("On") == "MD"  staticruntime("Off") == "MT"] + [runtime "Debug/Release"] == "MDd"/"MTd"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader("pkpch.h")
	pchsource("Pika/src/pkpch.cpp")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PK_PLATFORM_WINDOWS",
			"PK_BUILD_DLL",
			-- we don not want GLFW include OpenGL function, glad got all of it!
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "PIKA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PIKA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PIKA_DIST"
		runtime "Release"
		optimize "On"

-- Sandbox
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"Pika/src",
		"%{wks.location}/Pika/vendor",
		"%{includeDir.glm}"
	}

	links
	{
		"Pika"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"PK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PIKA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PIKA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PIKA_DIST"
		runtime "Release"
		optimize "On"
	