#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Core/Timer.h"
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
		ImGuiLayer* m_pImGuiLayer; //only one ImGuiLayer instance, but each Layer can use ImGuiRender()
	private:
		Timer m_Timer;
		float m_LastFrameTime;

		static Application* s_pSingletonInstance;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  