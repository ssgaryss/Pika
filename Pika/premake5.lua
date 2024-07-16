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

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",  -- _CRT_SECURE_NO_WARNINGS 是一个预处理器定义，用于禁用 Microsoft Visual Studio 编译器对一些标准 C/C++ 函数（如 strcpy、sprintf 等）的安全性警告。
		"YAML_CPP_STATIC_DEFINE" -- use yaml-cpp as static lib instead of dll
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PK_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE" --(we don not want GLFW include OpenGL function, glad got all of it!)
			-- "PK_BUILD_DLL" --(Pika is a static lib instead of dll now)
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