#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Core/Window.h"
#include "Pika/Core/LayerStack.h"

namespace Pika {

	class PIKA_API Application
	{
	public:
		virtual ~Application() = default;
		void onEvent(Event& vEvent);
		void pushLayer(Layer* vLayer);
		void pushOverlay(Layer* vLayer);
		void run();

		static Application& getInstance();
		inline Window& getWindow() { return *m_Window; }
	protected:
		Application(); //Singleton pattern, can not use ctor
	private:
		bool onWindowCloseEvent(WindowCloseEvent& vEvent);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_pSingletonInstance;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  