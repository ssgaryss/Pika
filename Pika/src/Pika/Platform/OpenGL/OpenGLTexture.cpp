#include "pkpch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>


namespace Pika {

	OpenGLTexture::OpenGLTexture(const std::string& Path)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
	}

	uint32_t OpenGLTexture::getWidth() const
	{
		return 0;
	}

	uint32_t OpenGLTexture::getHeight() const
	{
		return 0;
	}

	uint32_t OpenGLTexture::getRendererID() const
	{
		return 0;
	}

	const std::string& OpenGLTexture::getPath() const
	{
		// TODO: 在此处插入 return 语句
		return "1";
	}

	void OpenGLTexture::bind() const
	{
	}

	void OpenGLTexture::unbind() const
	{
	}

}