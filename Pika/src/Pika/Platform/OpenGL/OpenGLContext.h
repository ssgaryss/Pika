#pragma once
#include "Pika/Renderer/GraphicsContext.h"

struct GLFWwindow; //Forward Declaration

namespace Pika
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext() = default;
		OpenGLContext(GLFWwindow* vWindow);
		void init() override;
		void swapBuffer() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}

