#include "pkpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"


namespace Pika
{
	void OpenGLRendererAPI::init()
	{
		PK_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);  //MSAA
		
	}
	void Pika::OpenGLRendererAPI::clear()
	{
		PK_PROFILE_FUNCTION();

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
		PK_PROFILE_FUNCTION();

		vVertexArray->bind();
		glDrawElements(GL_TRIANGLES, vVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		vVertexArray->unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t OpenGLRendererAPI::getAvailableTextureSlots()
	{
		GLint maxTextureUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		return static_cast<uint32_t>(maxTextureUnits);
	}

}
