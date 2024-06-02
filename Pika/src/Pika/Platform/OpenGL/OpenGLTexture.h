#pragma once
#include "Pika/Renderer/Texture.h"

namespace Pika
{

	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& Path);
		~OpenGLTexture();
		uint32_t getWidth() const override;
		uint32_t getHeight() const override;
		uint32_t getRendererID() const override;
		const std::string& getPath() const override;

		void bind() const override;
		void unbind() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Path;
	};

}
