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
includeDir["stb_image"] = "Pika/vendor/stb_image"

group "Dependencies"
	include "Pika/vendor/GLFW"
	include "Pika/vendor/glad"
	include "Pika/vendor/imgui"
group ""

-- Pika
project "Pika"
	location "Pika"
	kind "Staticlib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" -- buildoptions "/MDd" --> [staticruntime("On") == "MD"  staticruntime("Off") == "MT"] + [runtime "Debug/Release"] == "MDd"/"MTd"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader("pkpch.h")
	pchsource("Pika/src/pkpch.cpp")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	defines "_CRT_SECURE_NO_WARNINGS"

	filter "system:windows"
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
	cppdialect "C++17"
	staticruntime "on"

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
	