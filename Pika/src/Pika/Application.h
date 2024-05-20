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
		void run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  