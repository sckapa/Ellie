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
 IncludeDir["GLFW"] = "Ellie/vendor/GLFW/include"
 IncludeDir["Glad"] = "Ellie/vendor/Glad/include"
 IncludeDir["ImGui"] = "Ellie/vendor/imgui"
 IncludeDir["glm"] = "Ellie/vendor/glm"
 IncludeDir["stb_image"] = "Ellie/vendor/stb_image"
 IncludeDir["entt"] = "Ellie/vendor/entt/include"
 IncludeDir["yaml_cpp"] = "Ellie/vendor/yaml-cpp/include"
 
 group "Dependencies"
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
  "%{prj.name}/vendor/imgui/imgui_tables.cpp"
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
  "%{IncludeDir.GLFW}",
  "%{IncludeDir.Glad}",
  "%{IncludeDir.ImGui}",
  "%{IncludeDir.stb_image}",
  "%{IncludeDir.glm}",
  "%{IncludeDir.yaml_cpp}",
  "%{IncludeDir.entt}"
 }

 links
 {
  "GLFW",
  "Glad",
  "ImGui",
  "yaml-cpp",
  "opengl32.lib"
 }

 filter "system:windows"
  systemversion "latest"

  defines
  {
   "EE_PLATFORM_WINDOWS",
   "EE_BUILD_DLL",
   "GLFW_INCLUDE_NONE",
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
  "%{IncludeDir.entt}"
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