#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Core/Window.h"
#include "Pika/Core/LayerStack.h"
#include "Pika/ImGui/ImGuiLayer.h"

#include "Pika/Renderer/VertexArray.h"
#include "Pika/Renderer/Shader.h"
#include "Pika/Renderer/Buffer.h"
#include "Pika/Renderer/Renderer.h"
#include "Pika/Renderer/RenderCommand.h"

namespace Pika {

	class Application
	{
	public:
		virtual ~Application() = default;
		void onEvent(Event& vEvent);
		void pushLayer(Layer* vLayer);
		void pushOverlay(Layer* vLayer);
		void run();

		static Application& getInstance(); //s_pSingletonInstance
		inline Window& getWindow() { return *m_Window; }
	protected:
		Application(); //Singleton pattern, can not use ctor
	private:
		bool onWindowCloseEvent(WindowCloseEvent& vEvent);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_pImGuiLayer; //only one ImGuiLayer instance, but each Layer can use ImGuiRender()
		static Application* s_pSingletonInstance;


		std::shared_ptr<VertexArray> VAO_1;
		std::shared_ptr<VertexArray> VAO_2;
		std::shared_ptr<VertexBuffer> VBO_1;
		std::shared_ptr<VertexBuffer> VBO_2;
		std::shared_ptr<VertexBuffer> VBO_3;
		std::shared_ptr<IndexBuffer> EBO;
		std::shared_ptr<Shader> shader_1;
		std::shared_ptr<Shader> shader_2;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  