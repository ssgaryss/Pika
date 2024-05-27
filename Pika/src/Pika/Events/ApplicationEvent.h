#pragma once

#include "pkpch.h"
#include "Pika/Events/Event.h"

namespace Pika {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent() = delete;
		WindowResizeEvent(unsigned int vWidth, unsigned int vHeight) :
			m_Width{ vWidth }, m_Height{ vHeight } {};
		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		unsigned int m_Width, m_Height;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

}