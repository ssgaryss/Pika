project "Sandbox"
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

	removefiles
    {
		"src/**.h",
		"src/**.cpp"  -- 暂时不需要Sandbox
    }

	includedirs
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
		"GLM_ENABLE_EXPERIMENTAL" -- 允许使用glm/gtx内容
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
	