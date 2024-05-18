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

#define BIT(x) (1 << x)