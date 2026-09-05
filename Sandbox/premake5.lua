project "Sandbox"
	kind "None" -- Sandbox has no sources yet (removefiles below); "None" skips the link step
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

	removefiles
    {
		"src/**.h",
		"src/**.cpp"  -- Sandbox is not needed yet
    }

	sysincludedirs
	{
		"%{wks.location}/Pika/vendor/spdlog/include",
		"%{wks.location}/Pika/src",
		"%{wks.location}/Pika/vendor",
		"%{includeDir.glm}",
		"%{includeDir.entt}"
	}

	links
	{
		"Pika"
	}

	defines
	{
		"GLM_ENABLE_EXPERIMENTAL" -- enable glm/gtx helpers
	}

	filter "system:windows"
		systemversion "latest"
		defines { "PK_PLATFORM_WINDOWS" }
		buildoptions { "/utf-8" } -- MSVC: read source as UTF-8

	filter "system:macosx"
		defines { "PK_PLATFORM_MACOS", "SPDLOG_USE_STD_FORMAT" } -- bundled fmt breaks on clang 16+

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
