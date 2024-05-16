#pragma once

#include "Core.h"

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
  