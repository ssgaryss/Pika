#pragma once

#ifdef PK_PLATFORM_WINDOWS

extern Pika::Application* Pika::createApplication();

int main(int argc, char** argv)
{
	auto app{ Pika::createApplication() };
	app->run();
	delete app;
}
#else
#error Pika only for Windows! 
#endif // PK_PLATFORM_WINDOWS

