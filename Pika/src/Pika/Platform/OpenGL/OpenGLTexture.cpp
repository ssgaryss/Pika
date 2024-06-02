#include "pkpch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>


namespace Pika {

	OpenGLTexture::OpenGLTexture(const std::string& vPath)
		: m_Path{ vPath }
	{
		int Width, Height, Channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* Data = stbi_load(vPath.c_str(), &Width, &Height, &Channels, 0); //0 means desired channels = Channels
		PK_CORE_ASSERT(Data, "Texture Loading is failed!");
		m_Width = Width;
		m_Height = Height;

		GLenum InternalFormat = 0, DataFormat = 0;
		if (Channels == 4) {
			InternalFormat = GL_RGBA8;
			DataFormat = GL_RGBA;
		}
		else if (Channels == 3) {
			InternalFormat = GL_RGB8;
			DataFormat = GL_RGB;
		}
		m_InternalFormat = InternalFormat;
		m_DataFormat = DataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, DataFormat, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::bind(uint32_t vSlot) const
	{
		glBindTextureUnit(vSlot, m_RendererID);
	}

	void OpenGLTexture::unbind(uint32_t vSlot) const
	{
		glBindTextureUnit(vSlot, 0);
	}

}