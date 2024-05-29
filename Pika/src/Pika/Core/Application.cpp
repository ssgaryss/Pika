#include "pkpch.h"
#include "Application.h"
#include "Pika/Core/Input.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "Pika/Renderer/Buffer.h"
#include "Pika/Renderer/Shader.h"

namespace Pika {
	Application* Application::s_pSingletonInstance = nullptr;

	Application::Application()
	{
		PK_ASSERT(!s_pSingletonInstance, "Application already exists!");
		s_pSingletonInstance = this;
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		const char* Name = R"(Colored shader)";

		const char* VertexShader = R"(
		#version 460 core
		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec3 a_Color;

		out vec4 v_Color;
			
		void main(){
			gl_Position = vec4(a_Position, 1.0f);
			v_Color = vec4(a_Color, 1.0f);
		})";

		const char* FragentShader = R"(
		out vec4 FragmentColor;

		in vec4 v_Color;
		
		void main(){
			FragmentColor = v_Color;
		})";

		shader_1 = Shader::create(Name, VertexShader, FragentShader);
		shader_1->bind();

		//glGenVertexArrays(1, &m_VertexArray);
		//glBindVertexArray(m_VertexArray);
		VAO.reset(VertexArray::create());
		VAO->bind();

		float vertices[4 * 6]
		{
			 0.5f,  0.5f, 0.0f, 0.8f, 0.2f, 0.2f, // top right
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.2f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, // bottom left
			-0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f   // top left 
		};

		//glGenBuffers(1, &m_VertexBuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		VBO.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		PK_ASSERT(VBO, "VertexBuffer is nullptr!");

		BufferLayout Layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Color"}
		};
		VBO->setLayout(Layout);
		VAO->addVertexBuffer(VBO);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		uint32_t indices[6]{
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle 
		};

		//glGenBuffers(1, &m_IndexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		EBO.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		VAO->setIndexBuffer(EBO);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		VBO->unbind();
		VAO->unbind();
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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			VAO->bind();
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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