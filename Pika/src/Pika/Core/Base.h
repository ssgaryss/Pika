#pragma once

// Platform detection: exactly one of PK_PLATFORM_WINDOWS / PK_PLATFORM_MACOS is
// injected by premake5 (see premake5.lua). Adding a new platform means adding
// the matching define plus a branch here.
#if defined(PK_PLATFORM_WINDOWS)
	// Pika is currently built as a static library, so PIKA_API is empty. The
	// DLL import/export branch is kept for a future shared-library build.
	#ifdef PK_BUILD_DLL
		#define PIKA_API __declspec(dllexport)
	#else
		#define PIKA_API __declspec(dllimport)
	#endif
#elif defined(PK_PLATFORM_MACOS)
	#define PIKA_API
#else
	#error "Pika: unknown platform (expected PK_PLATFORM_WINDOWS or PK_PLATFORM_MACOS)"
#endif

#ifdef PIKA_DEBUG
	#define PK_ENABLE_ASSERTS
#endif

// Break into the debugger in a toolchain-agnostic way.
#if defined(_MSC_VER)
	#define PK_DEBUGBREAK() __debugbreak()
#else
	#define PK_DEBUGBREAK() __builtin_trap()
#endif

#ifdef PK_ENABLE_ASSERTS
	#define PK_ASSERT(x, ...)      { if (!(x)) { PK_ERROR(__VA_ARGS__);      PK_DEBUGBREAK(); } }
	#define PK_CORE_ASSERT(x, ...) { if (!(x)) { PK_CORE_ERROR(__VA_ARGS__); PK_DEBUGBREAK(); } }
#else
	#define PK_ASSERT(x, ...)
	#define PK_CORE_ASSERT(x, ...)
#endif

#include <memory>
namespace Pika {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... vArgs) {
		return std::make_unique<T>(std::forward<Args>(vArgs)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... vArgs) {
		return std::make_shared<T>(std::forward<Args>(vArgs)...);
	}

}
