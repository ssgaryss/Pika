#include "pkpch.h"
#include "Application.h"
#include "Pika/Core/Input.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

namespace Pika {
	Application* Application::s_pSingletonInstance = nullptr;

	Application::Application()
	{
		PK_ASSERT(!s_pSingletonInstance, "Application already exists!");
		s_pSingletonInstance = this;
		m_Window = std::unique_ptr<Window>(Window::Ccreate());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

	}

	void Application::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowCloseEvent, this, std::placeholders::_1));
		//PK_CORE_TRACE(vEvent.toString()); //trace all events
		//std::cout << vEvent << "\n";

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
			for (auto& it : m_LayerStack) {
				it->onUpdate();
			}

			m_pImGuiLayer->begin();
			for (auto it : m_LayerStack)
			{
				it->onImGuiRender();
			}
			static bool Show = true;
			ImGui::ShowDemoWindow(&Show);
			m_pImGuiLayer->end();

			m_Window->onUpdate();

		}
	}
	Application& Application::getInstance()
	{
		return *s_pSingletonInstance;
	}
	bool Application::onWindowCloseEvent(WindowCloseEvent& vEvent)
	{
		m_Running = false;
		return true;
	}

}