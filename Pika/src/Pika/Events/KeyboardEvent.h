#pragma once
#include "Pika/Events/Event.h"
#include "Pika/Core/KeyCodes.h"
#include <sstream>

namespace Pika {

	class KeyEvent : public Event {
	public:
		inline Key::KeyCode getKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(const Key::KeyCode vKeyCode) :
			m_KeyCode{ vKeyCode } {}
		Key::KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const Key::KeyCode vKeyCode, bool vIsRepeat = false) :
			KeyEvent{ vKeyCode }, m_IsRepeat{ vIsRepeat } {}
		inline bool isRepeat() const { return m_IsRepeat; }

		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << static_cast<uint16_t>(m_KeyCode) << " (" << m_IsRepeat << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const Key::KeyCode vKeyCode) :
			KeyEvent{ vKeyCode } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << static_cast<uint16_t>(m_KeyCode);
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const Key::KeyCode vKeycode) :
			KeyEvent{ vKeycode } {}
		inline std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << static_cast<uint16_t>(m_KeyCode);
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}