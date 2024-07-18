#pragma once
#include "Event.h"
#include "Pika/Core/MouseCodes.h"
#include <sstream>

namespace Pika {

	class MouseButtonEvent : public Event
	{
	public:
		inline Mouse::MouseCode getMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);
	protected:
		MouseButtonEvent(const Mouse::MouseCode vButton) :
			m_Button{ vButton } {}
		Mouse::MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const Mouse::MouseCode vButton) :
			MouseButtonEvent{ vButton } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << static_cast<uint16_t>(m_Button);
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class MouseButtonRleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonRleasedEvent(const Mouse::MouseCode vButton) :
			MouseButtonEvent{ vButton } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonRleasedEvent: " << static_cast<uint16_t>(m_Button);
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonRleased);
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float vMouseX, float vMouseY) :
			m_MouseX{ vMouseX }, m_MouseY{ vMouseY } {}

		inline float getMouseX() const { return m_MouseX; }
		inline float getMouseY() const { return m_MouseY; }

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

	class MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float vXOffset, float vYOffset) :
			m_XOffset{ vXOffset }, m_YOffset{ vYOffset } {}

		inline float getXOffet() const { return m_XOffset; }
		inline float getYOffet() const { return m_YOffset; }

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