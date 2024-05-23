#pragma once

#ifdef PK_PLATFORM_WINDOWS
	#ifdef PK_BUILD_DLL
		#define PIKA_API __declspec(dllexport)
	#else
		#define PIKA_API __declspec(dllimport)
	#endif
#else
	#error Pika only for Windows! 
#endif

#ifdef PIKA_DEBUG
	#define PK_ENABLE_ASSERTS
#endif


#ifdef PK_ENABLE_ASSERTS
	#define PK_ASSERT(x, ...) { if(!(x)){ PK_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define PK_CORE_ASSERT(x, ...) { if(!(x)){ PK_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define PK_ASSERT(x, ...)
	#define PK_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)