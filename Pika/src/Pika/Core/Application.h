#pragma once

#include "Base.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Core/Timer.h"
#include "Pika/Core/Window.h"
#include "Pika/Core/LayerStack.h"
#include "Pika/ImGui/ImGuiLayer.h"

namespace Pika {

	struct ApplicationSpecification
	{
		std::string m_AppName = "Pika Engine";
	};

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
		inline void close() { m_IsRunning = false; }

		inline Window& getWindow() { return *m_Window; }
		inline ImGuiLayer* getImGuiLayer() { return m_pImGuiLayer; }

		static Application& GetInstance(); //s_pSingletonInstance
	protected:
		Application(const ApplicationSpecification& vApplicationSpecification); //Singleton pattern, can not use ctor
	private:
		bool onWindowCloseEvent(WindowCloseEvent& vEvent);
		bool onWindowResizeEvent(WindowResizeEvent& vEvent);
		bool onMouseMovedEvent(MouseMovedEvent& vEvent); // TODO : Use to debug for now!
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
  