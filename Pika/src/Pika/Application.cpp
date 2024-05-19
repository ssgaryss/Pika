#include "pkpch.h"
#include "Application.h"

#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Log.h"

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
		PK_TRACE(w.toString());
		while (true)
		{

		}
	}
}