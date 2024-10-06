#pragma once
#include "Pika/Renderer/Texture.h"
#include <glad/glad.h>

namespace Pika
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& vTextureSpecification);
		OpenGLTexture2D(const std::filesystem::path& vPath, bool vRequiredMips = true);
		~OpenGLTexture2D();
		inline uint32_t getWidth() const override { return m_Width; }
		inline uint32_t getHeight() const override { return m_Height; }
		inline uint32_t getRendererID() const override { return m_RendererID; }
		inline const std::filesystem::path& getPath() const override { return m_Path; }
		inline bool getIsLoaded() const override { return m_IsLoaded; }
		void setData(void* vData, uint32_t vSize) override;

		void bind(uint32_t vSlot = 0) const override;
		void unbind(uint32_t vSlot = 0) const override;
		inline bool operator==(const Texture& rhs) const override {
			return m_RendererID == dynamic_cast<const OpenGLTexture2D&>(rhs).getRendererID();
		}
	private:
		void loadTexture(const std::filesystem::path& vPath);
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		std::filesystem::path m_Path;
		bool m_RequiredMips = true;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat; // InternalFormat : GPU side, DataFormat : CPU side
	};

	class OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const TextureSpecification& vTextureSpecification);
		OpenGLCubemap(const std::filesystem::path& vPath, bool vRequiredMips = true);
		~OpenGLCubemap();
		inline uint32_t getWidth() const override { return m_Width; }
		inline uint32_t getHeight() const override { return m_Height; }
		inline uint32_t getRendererID() const override { return m_RendererID; }
		inline const std::filesystem::path& getPath() const override { return m_Path; }
		inline bool getIsLoaded() const override { return m_IsLoaded; }
		void setData(void* vData, uint32_t vSize) override;

		void bind(uint32_t vSlot = 0) const override;
		void unbind(uint32_t vSlot = 0) const override;
		inline bool operator==(const Texture& rhs) const override {
			return m_RendererID == dynamic_cast<const OpenGLCubemap&>(rhs).getRendererID();
		}
	private:
		void loadCubemap(const std::filesystem::path& vPath);
		void loadHDR(const std::filesystem::path& vPath);
		void loadLDR(const std::filesystem::path& vPath);
		void convertPanoramaToCubemap(); // TODO !
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		std::filesystem::path m_Path;
		bool m_RequiredMips = true;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat; // InternalFormat : GPU side, DataFormat : CPU side
	};

}
