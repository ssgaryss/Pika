#include "pkpch.h"
#include "Application.h"

namespace Pika {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::create());
	}
	Application::~Application()
	{
	}
	void Application::run()
	{
		WindowResizeEvent w(1920, 1080);
		PK_TRACE(w.toString());
		while (true)
		{
			m_Window->onUpdate();
		}
	}
}