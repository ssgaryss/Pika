#pragma once

#include "Event.h"
#include <sstream>

namespace Pika {

	class PIKA_API MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);
	protected:
		MouseButtonEvent(int vButton) :
			m_Button{ vButton } {}
		int m_Button;
	};

	class PIKA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int vButton) :
			MouseButtonEvent{ vButton } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class PIKA_API MouseButtonRleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonRleasedEvent(int vButton) :
			MouseButtonEvent{ vButton } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonRleasedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonRleased);
	};

	class PIKA_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float vMouseX, float vMouseY) :
			m_MouseX{ vMouseX }, m_MouseY{ vMouseY } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
	private:
		float m_MouseX, m_MouseY;
	};

	class PIKA_API MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float vXOffset, float vYOffset) :
			m_XOffset{ vXOffset }, m_YOffset{ vYOffset } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
	private:
		float m_XOffset, m_YOffset;
	};
}