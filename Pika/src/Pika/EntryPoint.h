#pragma once

#ifdef PK_PLATFORM_WINDOWS

extern Pika::Application* Pika::createApplication();

int main(int argc, char** argv)
{
	Pika::Log::init();
	PK_CORE_TRACE("trace");
	PK_CORE_INFO("info");
	PK_CORE_WARN("warn");
	PK_CORE_ERROR("error");
	PK_CORE_FATAL("fatal");
	int a = 10;
	PK_TRACE("trace var = {0}", a);
	PK_INFO("info");
	PK_WARN("warn");
	PK_ERROR("error");
	PK_FATAL("fatal");
	auto app{ Pika::createApplication() };
	app->run();
	delete app;
}
#else
#error Pika only for Windows! 
#endif // PK_PLATFORM_WINDOWS

