workspace "Ellie"
	architecture "x64"
	startproject "Ellie-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["Box2D"] = "Ellie/vendor/Box2D/include"
	IncludeDir["GLFW"] = "Ellie/vendor/GLFW/include"
	IncludeDir["Glad"] = "Ellie/vendor/Glad/include"
	IncludeDir["ImGui"] = "Ellie/vendor/imgui"
	IncludeDir["glm"] = "Ellie/vendor/glm"
	IncludeDir["stb_image"] = "Ellie/vendor/stb_image"
	IncludeDir["entt"] = "Ellie/vendor/entt/include"
	IncludeDir["yaml_cpp"] = "Ellie/vendor/yaml-cpp/include"
	IncludeDir["ImGuizmo"] = "Ellie/vendor/ImGuizmo"
	IncludeDir["Mono"] = "Ellie/vendor/mono/include"
	IncludeDir["filewatch"] = "Ellie/vendor/filewatch"

	group "Dependencies"
		include "Ellie/vendor/Box2D"
		include "Ellie/vendor/GLFW"
		include "Ellie/vendor/Glad"
		include "Ellie/vendor/imgui"
		include "Ellie/vendor/yaml-cpp"
	group ""

project "Ellie"
	location "Ellie"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "eepch.h"
	pchsource "Ellie/src/eepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/imgui/imgui_tables.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Mono}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.filewatch}"
	}

	links
	{
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"Ellie/vendor/mono/lib/%{cfg.buildcfg}/libmono-static-sgen.lib"
	}

	filter "files:Ellie/vendor/ImGuizmo/**.cpp"
		flags {"NoPCH"}

	filter "files:Ellie/vendor/ImGui/**.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

	defines
	{
		"EE_PLATFORM_WINDOWS",
		"EE_BUILD_DLL",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	links
	{
		"Ws2_32.lib",
		"Winmm.lib",
		"Version.lib",
		"Bcrypt.lib"
	}

	filter "configurations:Debug"
		defines "EE_DEBUG"
		symbols "on"
		buildoptions "/utf-8"
		runtime "Debug"

	filter "configurations:Release"
		defines "EE_RELEASE"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

	filter "configurations:Dist"
		defines "EE_DIST"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

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
		"Ellie/vendor/spdlog/include",
		"Ellie/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Ellie"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

	defines
	{
		"EE_PLATFORM_WINDOWS",
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "configurations:Debug"
		defines "EE_DEBUG"
		symbols "on"
		buildoptions "/utf-8"
		runtime "Debug"

	filter "configurations:Release"
		defines "EE_RELEASE"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

	filter "configurations:Dist"
		defines "EE_DIST"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

project "Ellie-Editor"
	location "Ellie-Editor"
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
		"Ellie/vendor/spdlog/include",
		"Ellie/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.filewatch}"
	}

	links
	{
		"Ellie"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

	defines
	{
		"EE_PLATFORM_WINDOWS",
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "configurations:Debug"
		defines "EE_DEBUG"
		symbols "on"
		buildoptions "/utf-8"
		runtime "Debug"

	filter "configurations:Release"
		defines "EE_RELEASE"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

	filter "configurations:Dist"
		defines "EE_DIST"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"

project "Ellie-ScriptCore"
	location "Ellie-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Ellie-Editor/Resources/Scripts")
	objdir ("Ellie-Editor/Resources/Scripts/Intermediates")

	files
	{
		"%{prj.name}/Source/**.cs",
		"%{prj.name}/Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"