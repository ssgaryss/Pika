project "Pika"
	kind "Staticlib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
	}

	-- Platform-specific backends live under src/Pika/Platform/<OS>/. They are
	-- excluded from the generic glob above and re-added per-OS below so only the
	-- matching implementation is compiled.
	removefiles
	{
		"src/Pika/Platform/Windows/**.cpp",
	}

	externalincludedirs
	{
		"vendor/spdlog/include",
		"src",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}",
		"%{includeDir.yaml_cpp}",
		"%{includeDir.ImGuizmo}",
		"%{includeDir.Box2D}",
		"%{includeDir.assimp}"
	}

	links
	{
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
		"GLFW_INCLUDE_NONE",        -- stop glfw3.h pulling in gl.h; glad owns GL loading (all platforms)
		"_CRT_SECURE_NO_WARNINGS",  -- MSVC-only, ignored by other toolchains
		"YAML_CPP_STATIC_DEFINE",   -- use yaml-cpp as a static library
		"GLM_ENABLE_EXPERIMENTAL"   -- enable glm/gtx helpers
	}

	filter "system:windows"
		systemversion "latest"
		defines { "PK_PLATFORM_WINDOWS" }
		buildoptions { "/utf-8" } -- MSVC: read source as UTF-8 (source files are UTF-8)
		links { "opengl32.lib" }
		files { "src/Pika/Platform/Windows/**.cpp" }

	filter "system:macosx"
		defines { "PK_PLATFORM_MACOS", "SPDLOG_USE_STD_FORMAT" } -- bundled fmt breaks on clang 16+; std::format works
		-- GLFW's Cocoa backend needs these frameworks at link time. Premake
		-- propagates a static library's links to any executable that links it.
		links
		{
			"Cocoa.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"QuartzCore.framework"
		}
		files { "src/Pika/Platform/MacOS/**.mm" }

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
