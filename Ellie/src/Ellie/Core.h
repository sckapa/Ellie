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

#ifdef EE_DEBUG
	#define EE_ENABLE_ASSERTS
#endif

#ifdef EE_ENABLE_ASSERTS

	#define EE_ASSERT(x, ...) { if(!(x)) {EE_ERROR("Assertion failed : {0}", __VA_ARGS__); __debugbreak();} }
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) {EE_CORE_ERROR("Assertion failed : {0}", __VA_ARGS__); __debugbreak();} }

#else
	#define EE_ASSERT(x, ...)
	#define EE_CORE_ASSERT(x, ...)

#endif 

#define BIT(x) (1 << x)