project "Glad"
	kind "StaticLib"
	language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/Glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
            systemversion "latest"

	filter "configurations:Debug"
			symbols "on"
			buildoptions "/utf-8"
			runtime "Debug"

	filter "configurations:Release"
		optimize "on"
		buildoptions "/utf-8"
		runtime "Release"