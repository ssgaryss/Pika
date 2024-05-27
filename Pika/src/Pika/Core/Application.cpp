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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3]
		{
			-0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &m_IndexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			for (auto it : m_LayerStack) {
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