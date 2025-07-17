#include "pkpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"


namespace Pika
{
	void OpenGLRendererAPI::initialize(uint32_t vFlags)
	{
		PK_PROFILE_FUNCTION();

		if (vFlags & RendererAPI::EnableBlend) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else glDisable(GL_BLEND);
		if (vFlags & RendererAPI::EnableDepthTest) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
		if (vFlags & RendererAPI::EnableLineSmooth) glEnable(GL_LINE_SMOOTH);  //MSAA
		else glDisable(GL_LINE_SMOOTH);
		if (vFlags & RendererAPI::EnableCullBackFace) glEnable(GL_CULL_FACE);  // Culling
		else glDisable(GL_CULL_FACE);
	}
	void Pika::OpenGLRendererAPI::clear()
	{
		PK_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& vColor)
	{
		glClearColor(vColor.r, vColor.g, vColor.b, vColor.a);
	}

	void OpenGLRendererAPI::drawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount)
	{
		PK_PROFILE_FUNCTION();

		vVertexArray->bind();
		// vIndexCount不能超过VAO的IndexBuffer最大限制
		uint32_t Count = std::clamp(vIndexCount, 0u, vVertexArray->getIndexBuffer()->getCount());
		glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, nullptr);
		vVertexArray->unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::drawLines(const VertexArray* vVertexArray, uint32_t vIndexCount)
	{
		PK_PROFILE_FUNCTION();

		vVertexArray->bind();
		// vIndexCount不能超过VAO的IndexBuffer最大限制
		uint32_t Count = std::clamp(vIndexCount, 0u, vVertexArray->getIndexBuffer()->getCount());
		glDrawElements(GL_LINES, Count, GL_UNSIGNED_INT, nullptr);
		vVertexArray->unbind();
	}

	void OpenGLRendererAPI::setLineThickness(float vThickness)
	{
		glLineWidth(vThickness);
	}

	uint32_t OpenGLRendererAPI::getAvailableTextureSlots()
	{
		GLint maxTextureUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		return static_cast<uint32_t>(maxTextureUnits);
	}

	void OpenGLRendererAPI::depthMask(bool vAllow)
	{
		if (vAllow)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE); // 禁止写入深度缓冲区
	}

}
