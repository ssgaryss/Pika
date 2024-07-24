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

	includedirs
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