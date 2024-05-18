#pragma once

#include "Core.h"
#include "Pika/Events/Event.h"

namespace Pika {

	class PIKA_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};

	// To be defined in CLIENT
	Application* createApplication();
}
  