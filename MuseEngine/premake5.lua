workspace "Muse"
	architecture "x86_64"
	startproject "TestGame"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Muse/vendor/GLFW/include"
IncludeDir["Glad"] = "Muse/vendor/Glad/include"
IncludeDir["ImGui"] = "Muse/vendor/imgui"
IncludeDir["glm"] = "Muse/vendor/glm"
IncludeDir["stb_image"] = "Muse/vendor/stb_image"
IncludeDir["rapidjson"] = "Muse/vendor/rapidjson/include"
IncludeDir["rttr"] = "Muse/vendor/rttr/include"

IncludeLibDir = {}
IncludeLibDir["rttr"] = "Muse/vendor/rttr/lib/"

group "Dependencies"
	include "Muse/vendor/GLFW"
	include "Muse/vendor/Glad"
	include "Muse/vendor/imgui"

group ""

project "Muse"
	location "Muse"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MusePCH.h"
	pchsource "Muse/src/MusePCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.rapidjson}",
		"%{IncludeDir.rttr}"
	}

	libdirs 
	{
		"%{IncludeLibDir.rttr}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MUSE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MUSE_DEBUG"
		runtime "Debug"
		symbols "on"
		links "rttr_core_lib_s_d.lib"

	filter "configurations:Release"
		defines "MUSE_RELEASE"
		runtime "Release"
		optimize "on"
		links "rttr_core_lib_s.lib"

	filter "configurations:Dist"
		defines "MUSE_DIST"
		runtime "Release"
		optimize "on"
		links "rttr_core_lib_s.lib"

project "TestGame"
	location "TestGame"
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
		"Muse/vendor/spdlog/include",
		"Muse/src",
		"Muse/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.rttr}",
	}

	links
	{
		"Muse"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "MUSE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MUSE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MUSE_DIST"
		runtime "Release"
		optimize "on"
