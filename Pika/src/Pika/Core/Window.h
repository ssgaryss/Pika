#pragma once
#include "pkpch.h"
#include "Pika/Events/Event.h"

namespace Pika {

	struct WindowProps
	{
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
		WindowProps(const std::string& vTitle = "Pika Engine",
			uint32_t vWidth = 1920,
			uint32_t vHeight = 1080) :
			m_Title{ vTitle }, m_Width{ vWidth }, m_Height{ vHeight } {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void onUpdate() = 0;

		virtual std::string getTitle() const = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;

		//window attributes
		virtual void setEventCallback(const EventCallbackFn& vEventCallback) = 0;
		virtual void setVSync(bool vEnable) = 0;
		virtual bool isVSync() const = 0;

		static Window* Ccreate(const WindowProps& vWindowProps = WindowProps());
	};

}