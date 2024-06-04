#include "pkpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"


namespace Pika
{
	void OpenGLRendererAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void Pika::OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::setClearColor()
	{
		auto Temp = Color();
		glClearColor(Temp.m_Red, Temp.m_Green, Temp.m_Blue, Temp.m_Alpha);
	}
	void OpenGLRendererAPI::setClearColor(Color vColor)
	{
		glClearColor(vColor.m_Red, vColor.m_Green, vColor.m_Blue, vColor.m_Alpha);
	}

	void OpenGLRendererAPI::drawIndexed(const VertexArray* vVertexArray)
	{
		vVertexArray->bind();
		glDrawElements(GL_TRIANGLES, vVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		vVertexArray->unbind();
	}

}
