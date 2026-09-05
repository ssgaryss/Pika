project "Pikartisan"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"src/**.h",
		"src/**.cpp"
	}

	sysincludedirs
	{
		"%{wks.location}/Pika/src",
		"%{wks.location}/Pika/vendor",
		"%{wks.location}/Pika/vendor/spdlog/include",
		"%{includeDir.glm}",
		"%{includeDir.entt}",
		"%{includeDir.ImGui}",
		"%{includeDir.ImGuizmo}"
	}

	links
	{
		"Pika",
		-- Pika is a static library; gmake does not propagate a static library's
		-- links to its consumers, so the executable lists the transitive deps.
		"GLFW",
		"glad",
		"ImGui",
		"yaml-cpp",
		"ImGuizmo",
		"Box2D",
		"assimp"
	}

	defines
	{
		"GLM_ENABLE_EXPERIMENTAL", -- enable glm/gtx helpers
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "PK_PLATFORM_WINDOWS" }
		buildoptions { "/utf-8" } -- MSVC: read source as UTF-8
		links { "opengl32.lib" }

	filter "system:macosx"
		defines { "PK_PLATFORM_MACOS", "SPDLOG_USE_STD_FORMAT" } -- bundled fmt breaks on clang 16+
		links
		{
			"Cocoa.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"QuartzCore.framework"
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
