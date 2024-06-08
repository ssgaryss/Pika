#include "pkpch.h"
#include "Application.h"
#include "Pika/Core/Input.h"
#include "Pika/Core/Timer.h"
#include "Pika/Core/Timestep.h"
#include "Pika/Renderer/Renderer3D.h"
#include "Pika/Renderer/RenderCommand.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

namespace Pika {
	Application* Application::s_pSingletonInstance = nullptr;

	Application::Application()
	{
		PK_PROFILE_FUNCTION();

		PK_ASSERT(!s_pSingletonInstance, "Application already exists!");
		s_pSingletonInstance = this;
		m_Window = Scope<Window>(Window::Create());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		Renderer3D::Init();
		m_pImGuiLayer = new ImGuiLayer();
		pushOverlay(m_pImGuiLayer);
	}

	void Application::onEvent(Event& vEvent)
	{
		PK_PROFILE_FUNCTION();

		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowCloseEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onWindowResizeEvent, this, std::placeholders::_1));
		//PK_CORE_TRACE(vEvent.toString()); //trace all events

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (vEvent.m_Handled) break;
			(*it)->onEvent(vEvent);
		}
	}

	void Application::run()
	{
		PK_PROFILE_FUNCTION();

		while (m_IsRunning)
		{
			PK_PROFILE_FUNCTION();

			float Time = m_Timer.elapsed();
			Timestep DeltaTime(Time - m_LastFrameTime);
			m_LastFrameTime = m_Timer.elapsed();

			{
				PK_PROFILE_SCOPE("Application : Renderer prep");

				RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
				RenderCommand::Clear();
			}

			if (!m_IsMinimized) {
				PK_PROFILE_SCOPE("Application : Layers update");

				for (auto& it : m_LayerStack) {
					it->onUpdate(DeltaTime);
				}

				m_pImGuiLayer->begin();
				for (auto it : m_LayerStack)
				{
					it->onImGuiRender();
				}
				m_pImGuiLayer->end();
			}

			m_Window->onUpdate();

		}
	}
	Application& Application::getInstance()
	{
		return *s_pSingletonInstance;
	}
	bool Application::onWindowCloseEvent(WindowCloseEvent& vEvent)
	{
		PK_PROFILE_FUNCTION();

		m_IsRunning = false;
		return true;
	}

	bool Application::onWindowResizeEvent(WindowResizeEvent& vEvent)
	{
		PK_PROFILE_FUNCTION();

		if (vEvent.getWidth() && vEvent.getHeight())
			m_IsMinimized = false;
		else
			m_IsMinimized = true;
		return false;
	}

}