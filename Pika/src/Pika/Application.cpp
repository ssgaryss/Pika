#include "pkpch.h"
#include "Application.h"
#include <GLFW/glfw3.h>

namespace Pika {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}
	Application::~Application()
	{
	}
	void Application::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		PK_CORE_TRACE(vEvent.toString());
	}
	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
		}
	}
	bool Application::onWindowClose(WindowCloseEvent& vEvent)
	{
		m_Running = false;
		return true;
	}
	bool Application::onWinodwResize(WindowResizeEvent& vEvent)
	{
		return false;
	}

}