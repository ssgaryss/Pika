#pragma once
#include "TextureFormat.h"


namespace Pika
{

	struct TextureSpecification {
		uint32_t m_Width = 1;
		uint32_t m_Height = 1;
		TextureFormat m_Format = TextureFormat::RGBA8;
		bool m_RequiredMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRendererID() const = 0;
		virtual const std::string& getPath() const = 0;
		virtual bool getIsLoaded() const = 0;
		virtual void setData(void* vData, uint32_t vSize) = 0;

		virtual void bind(uint32_t vSlot = 0) const = 0;
		virtual void unbind(uint32_t vSlot = 0) const = 0;
		virtual bool operator==(const Texture& rhs) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& vTextureSpecification);
		static Ref<Texture2D> Create(const std::filesystem::path& vPath, bool vRequiredMips = true);
	};

}

