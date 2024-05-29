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
		m_Window = std::unique_ptr<Window>(Window::Ccreate());
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

		const char* FragentShaderBlue = R"(
		out vec4 FragmentColor;

		in vec4 v_Color;
		
		void main(){
			FragmentColor = vec4(0.1f, 0.1f, 0.9f, 1.0f);
		})";

		shader_1 = Shader::Create(Name, VertexShader, FragentShader);
		shader_2 = Shader::Create(Name, VertexShader, FragentShaderBlue);


		//glGenVertexArrays(1, &m_VertexArray);
		//glBindVertexArray(m_VertexArray);
		VAO_1.reset(VertexArray::Create());
		VAO_1->bind();

		float vertices_pos[4 * 3]
		{
			 0.5f,  0.5f, 0.0f, // top right
			 0.5f, -0.5f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  // top left 
		};
		float vertices_color[4 * 3]
		{
			0.8f, 0.2f, 0.2f,
			0.2f, 0.8f, 0.2f,
			0.2f, 0.2f, 0.8f,
			0.5f, 0.5f, 0.5f
		};

		//glGenBuffers(1, &m_VertexBuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		VBO_1.reset(VertexBuffer::Create(vertices_pos, sizeof(vertices_pos)));
		VBO_2.reset(VertexBuffer::Create(vertices_color, sizeof(vertices_color)));
		BufferLayout Layout_1 = {
			{ShaderDataType::Float3, "a_Position"}
		};
		BufferLayout Layout_2 = {
			{ShaderDataType::Float3, "a_Color"}
		};
		VBO_1->setLayout(Layout_1);
		VBO_2->setLayout(Layout_2);
		VAO_1->addVertexBuffer(VBO_1);
		VAO_1->addVertexBuffer(VBO_2);
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
		EBO.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		VAO_1->setIndexBuffer(EBO);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		VBO_2->unbind();
		VBO_1->unbind();
		VAO_1->unbind();
		///////////////////////////////////////////////////////
		VAO_2.reset(VertexArray::Create());
		VAO_2->bind();
		float vertices[4 * 3]
		{
			 0.2f,  0.2f, 0.0f, // top right
			 0.2f, -0.2f, 0.0f, // bottom right
			-0.2f, -0.2f, 0.0f, // bottom left
			-0.2f,  0.2f, 0.0f,  // top left 
		};
		VBO_3.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout Layout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		VBO_3->setLayout(Layout);
		VAO_2->addVertexBuffer(VBO_3);
		VAO_2->setIndexBuffer(EBO);
		VAO_2->unbind();

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

			Renderer::BeginScene();
			Renderer::Submit(shader_1.get(), VAO_1.get());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
			Renderer::Submit(shader_2.get(), VAO_2.get());
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

			Renderer::EndScene();

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