workspace "Ellie"
 architecture "x64"

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

 include "Ellie/vendor/GLFW"
 include "Ellie/vendor/Glad"
 include "Ellie/vendor/imgui"

 project "Ellie"
 location "Ellie"
 kind "SharedLib"
 language "C++"

 targetdir ("bin/" .. outputdir .. "/%{prj.name}")
 objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

 pchheader "eepch.h"
 pchsource "Ellie/src/eepch.cpp"

 files
 {
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp"
 }

 includedirs
 {
  "%{prj.name}/src",
  "%{prj.name}/vendor/spdlog/include",
  "%{IncludeDir.GLFW}",
  "%{IncludeDir.Glad}",
  "%{IncludeDir.ImGui}"
 }

 links
 {
  "GLFW",
  "Glad",
  "ImGui",
  "opengl32.lib"
 }

 filter "system:windows"
  cppdialect "C++17"
  staticruntime "On"
  systemversion "latest"

  defines
  {
   "EE_PLATFORM_WINDOWS",
   "EE_BUILD_DLL",
   "GLFW_INCLUDE_NONE"
  }

  postbuildcommands
  {
   ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
  }

 filter "configurations:Debug"
  defines "EE_DEBUG"
  symbols "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Debug"

 filter "configurations:Release"
  defines "EE_RELEASE"
  optimize "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Release"

 filter "configurations:Dist"
  defines "EE_DIST"
  optimize "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Release"

project "Sandbox"
 location "Sandbox"
 kind "ConsoleApp"
 language "C++"

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
  "Ellie/src"
 }

 links
 {
  "Ellie"
 }

 filter "system:windows"
  cppdialect "C++17"
  staticruntime "On"
  systemversion "latest"

  defines
  {
   "EE_PLATFORM_WINDOWS"
  }

 filter "configurations:Debug"
  defines "EE_DEBUG"
  symbols "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Debug"

 filter "configurations:Release"
  defines "EE_RELEASE"
  optimize "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Release"

 filter "configurations:Dist"
  defines "EE_DIST"
  optimize "On"
  buildoptions "/utf-8"
  staticruntime "off"
  runtime "Release"