#pragma once
#include "Pika/Renderer/Texture.h"
#include <glad/glad.h>

namespace Pika
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& vPath);
		~OpenGLTexture2D();
		uint32_t getWidth() const override { return m_Width; }
		uint32_t getHeight() const override { return m_Height; }
		uint32_t getRendererID() const override { return m_RendererID; }
		const std::string& getPath() const override { return m_Path; }

		void bind(uint32_t vSlot = 0) const override;
		void unbind(uint32_t vSlot = 0) const override;
	private:
		void loadTexture(const std::string& vPath);
	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;

		std::string m_Path;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
