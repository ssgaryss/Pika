#pragma once
#include "Pika/Renderer/GraphicsContext.h"

struct GLFWwindow; //Forward Declaration

namespace Pika
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext() = delete;
		OpenGLContext(GLFWwindow* vWindow);
		~OpenGLContext() = default;
		void initialize() override;
		void swapBuffer() override;

	private:
		GLFWwindow* m_pWindowHandle; //GLFW window, the context should not delete this in destructor
	};

}

