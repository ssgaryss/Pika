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
		Application();
		virtual ~Application() = default;
		void onEvent(Event& vEvent);
		void pushLayer(Layer* vLayer);
		void pushOverlay(Layer* vLayer);
		void run();
	private:
		bool onWindowClose(WindowCloseEvent& vEvent);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  