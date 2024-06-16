#pragma once

#include "Base.h"
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
		inline void pushLayer(Layer* vLayer) { vLayer->onAttach(); m_LayerStack.pushLayer(vLayer); }
		inline void pushOverlay(Layer* vLayer) { vLayer->onAttach(); m_LayerStack.pushOverlay(vLayer); }
		inline void popLayer(Layer* vLayer) { vLayer->onDetach(); m_LayerStack.popLayer(vLayer); }
		inline void popOverlay(Layer* vLayer) { vLayer->onDetach(); m_LayerStack.popOverlay(vLayer); }

		void run();

		static Application& getInstance(); //s_pSingletonInstance
		inline Window& getWindow() { return *m_Window; }
	protected:
		Application(); //Singleton pattern, can not use ctor
	private:
		bool onWindowCloseEvent(WindowCloseEvent& vEvent);
		bool onWindowResizeEvent(WindowResizeEvent& vEvent);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
		bool m_IsMinimized = false;
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
  