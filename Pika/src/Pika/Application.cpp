#include "Application.h"

#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Log.h"
#include <iostream>

namespace Pika {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::run()
	{
		WindowResizeEvent w(1920, 1080);
		//std::cout << w;
		PK_TRACE(w);
		while (true)
		{

		}
	}
}