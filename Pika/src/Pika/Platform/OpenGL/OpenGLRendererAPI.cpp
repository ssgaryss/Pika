#include "pkpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"


namespace Pika
{
	void Pika::OpenGLRendererAPI::clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
