#pragma once


namespace Pika
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRendererID() const = 0;
		virtual const std::string& getPath() const = 0;

		virtual void bind(uint32_t vSlot = 0) const = 0;
		virtual void unbind(uint32_t vSlot = 0) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& vPath);
	};

}

