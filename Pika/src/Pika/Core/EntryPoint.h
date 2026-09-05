#pragma once
#include "Pika.h"

#include <clocale>
#include <locale>

extern Pika::Application* Pika::createApplication();

int main(int argc, char** argv)
{
	// On macOS, std::filesystem path -> string conversion throws "locale not
	// supported" for non-ASCII (e.g. CJK) paths unless a UTF-8 locale is active.
	// The empty-string locale can resolve back to "C", so try explicit UTF-8
	// locales first.
	static const char* s_UTF8Locales[] = { "en_US.UTF-8", "C.UTF-8", "" };
	for (const char* LocaleName : s_UTF8Locales) {
		if (setlocale(LC_ALL, LocaleName)) {
			try { std::locale::global(std::locale(LocaleName)); break; } catch (...) {}
		}
	}

	Pika::Log::Initialize();

	PK_PROFILE_BEGIN_SESSION("Startup", "PikaProfile-Startup.json");
	auto app{ Pika::createApplication() };
	PK_PROFILE_END_SESSION();

	PK_PROFILE_BEGIN_SESSION("Runtime", "PikaProfile-Runtime.json");
	app->run();
	PK_PROFILE_END_SESSION();

	PK_PROFILE_BEGIN_SESSION("Shutdown", "PikaProfile-Shutdown.json");
	delete app;
	PK_PROFILE_END_SESSION();
}
