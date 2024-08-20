#pragma once
#include "Pika.h"

#ifdef PK_PLATFORM_WINDOWS

extern Pika::Application* Pika::createApplication();

int main(int argc, char** argv)
{
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
#else
	#error Pika only for Windows for now ! 
#endif

