#pragma once

#ifdef EE_PLATFORM_WINDOWS

	#ifdef EE_BUILD_DLL
		#define ELLIE_API __declspec(dllexport)
	#else
		#define ELLIE_API __declspec(dllimport)
	#endif

#else
	#error Ellie only supports Windows!

#endif