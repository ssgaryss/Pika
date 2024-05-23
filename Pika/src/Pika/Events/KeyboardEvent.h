#pragma once

#include "Pika/Events/Event.h"
#include <sstream>

namespace Pika {

	class PIKA_API KeyEvent : public Event {
	public:
		inline int getKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(int vKeyCode) :
			m_KeyCode{ vKeyCode } {}
		int m_KeyCode;
	};

	class PIKA_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int vKeyCode, int vRepeatCount) :
			KeyEvent{ vKeyCode }, m_RepeatCount{ vRepeatCount } {}
		inline int getRepeatCount() const { return m_RepeatCount; }

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class PIKA_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int vKeyCode) :
			KeyEvent{ vKeyCode } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class PIKA_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int vKeycode) :
			KeyEvent{ vKeycode } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}