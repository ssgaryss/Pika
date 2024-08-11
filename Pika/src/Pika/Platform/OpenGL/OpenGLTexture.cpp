#include "pkpch.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureFormat.h"
#include <stb_image.h>

namespace Pika {

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& vTextureSpecification)
		: m_Width{ vTextureSpecification.m_Width }, m_Height{ vTextureSpecification.m_Height },
		m_RequiredMips{ vTextureSpecification.m_RequiredMips }
	{
		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(vTextureSpecification.m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(vTextureSpecification.m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (m_RequiredMips)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& vPath, bool vRequiredMips)
		: m_Path{ vPath }, m_RequiredMips{ vRequiredMips }
	{
		PK_PROFILE_FUNCTION();

		try
		{
			loadTexture(vPath);
			m_IsLoaded = true;
		}
		catch (const std::runtime_error& e)
		{
			PK_CORE_ERROR("OpenGLTexture2D : Fail to load texture at {0}", e.what());
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PK_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(void* vData, uint32_t vSize)
	{
		PK_PROFILE_FUNCTION();
		if (vData == nullptr || vSize == 0) {
			PK_CORE_WARN("OpenGLTexture2D : Set an empty texture data!");
			return;
		}
		uint32_t BPP = 0; //bytes per pixel
		if (m_DataFormat == GL_RED) BPP = 1;
		else if (m_DataFormat == GL_RGB) BPP = 3;
		else if (m_DataFormat == GL_RGBA) BPP = 4;
		PK_ASSERT(vSize == m_Width * m_Height * BPP, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, vData);
	}

	void OpenGLTexture2D::bind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, m_RendererID);
	}

	void OpenGLTexture2D::unbind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, 0);
	}

	void OpenGLTexture2D::loadTexture(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();

		int Width, Height, Channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* Data = stbi_load(vPath.string().c_str(), &Width, &Height, &Channels, 0); // 0 means desired channels = Channels
		if (!Data) throw std::runtime_error("Path : " + vPath.string());
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
		else if (Channels == 1) {
			InternalFormat = GL_RED;
			DataFormat = GL_RED;
		}
		m_InternalFormat = InternalFormat;
		m_DataFormat = DataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (m_RequiredMips)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, DataFormat, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);
		PK_CORE_INFO("OpenGLTexture2D : Success to load a texture at {0}.", vPath.string());
	}

}