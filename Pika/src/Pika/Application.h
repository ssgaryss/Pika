#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"
#include "Pika/Window.h"
#include "Pika/Events/ApplicationEvent.h"

namespace Pika {

	class PIKA_API Application
	{
	public:
		Application();
		virtual ~Application();
		void onEvent(Event& vEvent);
		void run();
	private:
		bool onWindowClose(WindowCloseEvent& vEvent);
		bool onWinodwResize(WindowResizeEvent& vEvent);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  