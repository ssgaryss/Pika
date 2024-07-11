project "Pika"
	kind "Staticlib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on" -- buildoptions "/MDd" --> [staticruntime("On") == "MD"  staticruntime("Off") == "MT"] + [runtime "Debug/Release"] == "MDd"/"MTd"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader("pkpch.h")
	pchsource("src/pkpch.cpp")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"vendor/spdlog/include",
		"src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}",
		"%{includeDir.yaml_cpp}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"yaml-cpp",
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