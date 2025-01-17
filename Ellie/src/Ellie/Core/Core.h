#pragma once
#include <memory>

#ifdef EE_PLATFORM_WINDOWS

#if EE_DYNAMIC_BUILD
	#ifdef EE_BUILD_DLL
		#define ELLIE_API __declspec(dllexport)
	#else
		#define ELLIE_API __declspec(dllimport)
	#endif
#else 
	#define ELLIE_API
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

#define EE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Ellie {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

}