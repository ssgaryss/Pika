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
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		PK_CORE_INFO("OpenGL Info :");
		PK_CORE_INFO("     Vendor : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_VENDOR)));
		PK_CORE_INFO("   Renderer : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_RENDERER)));
		PK_CORE_INFO("    Version : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_VERSION)));

	}

	void Application::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowCloseEvent, this, std::placeholders::_1));
		//PK_CORE_TRACE(vEvent.toString());

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
			for (auto it : m_LayerStack) {
				it->onUpdate();
			}

			m_ImGuiLayer->begin();
			for (auto it : m_LayerStack)
			{
				it->onImGuiRender();
			}
			static bool Show = true;
			ImGui::ShowDemoWindow(&Show);
			m_ImGuiLayer->end();
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