#include "pkpch.h"
#include "Application.h"
#include <glad/glad.h>

namespace Pika {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}

	void Application::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		PK_CORE_TRACE(vEvent.toString());

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (vEvent.m_Handled) break;
			(*it)->onEvent(vEvent);
		}
	}

	void Application::pushLayer(Layer* vLayer)
	{
		m_LayerStack.pushLayer(vLayer);
	}
	void Application::pushOverlay(Layer* vLayer)
	{
		m_LayerStack.pushOverlay(vLayer);
	}
	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
			for (auto it : m_LayerStack) {
				it->onUpdate();
			}
		}
	}
	bool Application::onWindowClose(WindowCloseEvent& vEvent)
	{
		m_Running = false;
		return true;
	}

}