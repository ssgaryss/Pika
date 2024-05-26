#include "pkpch.h"
#include "GraphicsContext.h"
#include "Pika/Platform/OpenGL/OpenGLContext.h"


namespace Pika
{
	GraphicsContext& GraphicsContext::createContext(void* vWindow)
	{
		return OpenGLContext(reinterpret_cast<GLFWwindow*>(vWindow));
	}
}
