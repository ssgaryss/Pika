#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Core/Window.h"
#include "Pika/Core/LayerStack.h"
#include "Pika/ImGui/ImGuiLayer.h"

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
		ImGuiLayer* m_ImGuiLayer; //only one ImGuiLayer instance, but each Layer can use ImGuiRender()
		static Application* s_pSingletonInstance;
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer; //VAO VBO EBO
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  