workspace "Ellie"
 architecture "x64"

 configurations
 {
  "Debug",
  "Release",
  "Dist"
 }

 outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

 project "Ellie"
 location "Ellie"
 kind "SharedLib"
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
  "%{prj.name}/vendor/spdlog/include"
 }

 filter "system:windows"
  cppdialect "C++17"
  staticruntime "On"
  systemversion "latest"

  defines
  {
   "EE_PLATFORM_WINDOWS",
   "EE_BUILD_DLL"
  }

  postbuildcommands
  {
   ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
  }

 filter "configurations:Debug"
  defines "EE_DEBUG"
  symbols "On"
  buildoptions "/utf-8"

 filter "configurations:Release"
  defines "EE_RELEASE"
  optimize "On"
  buildoptions "/utf-8"

 filter "configurations:Dist"
  defines "EE_DIST"
  optimize "On"
  buildoptions "/utf-8"

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

 filter "configurations:Release"
  defines "EE_RELEASE"
  optimize "On"
  buildoptions "/utf-8"

 filter "configurations:Dist"
  defines "EE_DIST"
  optimize "On"
  buildoptions "/utf-8"