#pragma once
#include "KeyCodes.h"
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

namespace Pika {

	static std::string KeyCodeToString(Key::KeyCode vKey) {
		using namespace Key;
		static std::unordered_map<Key::KeyCode, std::string> KeyString{
			{ KeyCode::Space, "Space" },
			{ KeyCode::Apostrophe, "Apostrophe" },
			{ KeyCode::Comma, "Comma" },
			{ KeyCode::Minus, "Minus" },
			{ KeyCode::Period, "Period" },
			{ KeyCode::Slash, "Slash" },
			{ KeyCode::D0, "D0" },
			{ KeyCode::D1, "D1" },
			{ KeyCode::D2, "D2" },
			{ KeyCode::D3, "D3" },
			{ KeyCode::D4, "D4" },
			{ KeyCode::D5, "D5" },
			{ KeyCode::D6, "D6" },
			{ KeyCode::D7, "D7" },
			{ KeyCode::D8, "D8" },
			{ KeyCode::D9, "D9" },
			{ KeyCode::Semicolon, "Semicolon" },
			{ KeyCode::Equal, "Equal" },
			{ KeyCode::A, "A" },
			{ KeyCode::B, "B" },
			{ KeyCode::C, "C" },
			{ KeyCode::D, "D" },
			{ KeyCode::E, "E" },
			{ KeyCode::F, "F" },
			{ KeyCode::G, "G" },
			{ KeyCode::H, "H" },
			{ KeyCode::I, "I" },
			{ KeyCode::J, "J" },
			{ KeyCode::K, "K" },
			{ KeyCode::L, "L" },
			{ KeyCode::M, "M" },
			{ KeyCode::N, "N" },
			{ KeyCode::O, "O" },
			{ KeyCode::P, "P" },
			{ KeyCode::Q, "Q" },
			{ KeyCode::R, "R" },
			{ KeyCode::S, "S" },
			{ KeyCode::T, "T" },
			{ KeyCode::U, "U" },
			{ KeyCode::V, "V" },
			{ KeyCode::W, "W" },
			{ KeyCode::X, "X" },
			{ KeyCode::Y, "Y" },
			{ KeyCode::Z, "Z" },
			{ KeyCode::LeftBracket, "LeftBracket" },
			{ KeyCode::Backslash, "Backslash" },
			{ KeyCode::RightBracket, "RightBracket" },
			{ KeyCode::GraveAccent, "GraveAccent" },
			{ KeyCode::World1, "World1" },
			{ KeyCode::World2, "World2" },
			{ KeyCode::Escape, "Escape" },
			{ KeyCode::Enter, "Enter" },
			{ KeyCode::Tab, "Tab" },
			{ KeyCode::Backspace, "Backspace" },
			{ KeyCode::Insert, "Insert" },
			{ KeyCode::Delete, "Delete" },
			{ KeyCode::Right, "Right" },
			{ KeyCode::Left, "Left" },
			{ KeyCode::Down, "Down" },
			{ KeyCode::Up, "Up" },
			{ KeyCode::PageUp, "PageUp" },
			{ KeyCode::PageDown, "PageDown" },
			{ KeyCode::Home, "Home" },
			{ KeyCode::End, "End" },
			{ KeyCode::CapsLock, "CapsLock" },
			{ KeyCode::ScrollLock, "ScrollLock" },
			{ KeyCode::NumLock, "NumLock" },
			{ KeyCode::PrintScreen, "PrintScreen" },
			{ KeyCode::Pause, "Pause" },
			{ KeyCode::F1, "F1" },
			{ KeyCode::F2, "F2" },
			{ KeyCode::F3, "F3" },
			{ KeyCode::F4, "F4" },
			{ KeyCode::F5, "F5" },
			{ KeyCode::F6, "F6" },
			{ KeyCode::F7, "F7" },
			{ KeyCode::F8, "F8" },
			{ KeyCode::F9, "F9" },
			{ KeyCode::F10, "F10" },
			{ KeyCode::F11, "F11" },
			{ KeyCode::F12, "F12" },
			{ KeyCode::F13, "F13" },
			{ KeyCode::F14, "F14" },
			{ KeyCode::F15, "F15" },
			{ KeyCode::F16, "F16" },
			{ KeyCode::F17, "F17" },
			{ KeyCode::F18, "F18" },
			{ KeyCode::F19, "F19" },
			{ KeyCode::F20, "F20" },
			{ KeyCode::F21, "F21" },
			{ KeyCode::F22, "F22" },
			{ KeyCode::F23, "F23" },
			{ KeyCode::F24, "F24" },
			{ KeyCode::F25, "F25" },
			{ KeyCode::KP0, "KP0" },
			{ KeyCode::KP1, "KP1" },
			{ KeyCode::KP2, "KP2" },
			{ KeyCode::KP3, "KP3" },
			{ KeyCode::KP4, "KP4" },
			{ KeyCode::KP5, "KP5" },
			{ KeyCode::KP6, "KP6" },
			{ KeyCode::KP7, "KP7" },
			{ KeyCode::KP8, "KP8" },
			{ KeyCode::KP9, "KP9" },
			{ KeyCode::KPDecimal, "KPDecimal" },
			{ KeyCode::KPDivide, "KPDivide" },
			{ KeyCode::KPMultiply, "KPMultiply" },
			{ KeyCode::KPSubtract, "KPSubtract" },
			{ KeyCode::KPAdd, "KPAdd" },
			{ KeyCode::KPEnter, "KPEnter" },
			{ KeyCode::KPEqual, "KPEqual" },
			{ KeyCode::LeftShift, "LeftShift" },
			{ KeyCode::LeftControl, "LeftControl" },
			{ KeyCode::LeftAlt, "LeftAlt" },
			{ KeyCode::LeftSuper, "LeftSuper" },
			{ KeyCode::RightShift, "RightShift" },
			{ KeyCode::RightControl, "RightControl" },
			{ KeyCode::RightAlt, "RightAlt" },
			{ KeyCode::RightSuper, "RightSuper" },
			{ KeyCode::Menu, "Menu" }
		};
		return KeyString[vKey];
	}

	class Shortcut
	{
	public:
		enum class ModifierType {
			None = 0,
			Ctrl = 1 << 0,
			Shift = 1 << 1,
			Alt = 1 << 2
		};
	public:
		Shortcut() = default;
		Shortcut(const std::string& vName, Key::KeyCode vKey, std::function<void()> vAction, int vModifierFlags = 0)
			: m_Name{ vName }, m_Key{ vKey }, m_Action{ vAction }, m_ModiferFlags{ vModifierFlags } {}
		inline const std::string& getActionName() const { return m_Name; }
		inline std::string toString() const {
			std::stringstream ss;
			if (m_ModiferFlags & static_cast<int>(ModifierType::Ctrl)) ss << "Ctrl + ";
			if (m_ModiferFlags & static_cast<int>(ModifierType::Shift)) ss << "Shift + ";
			if (m_ModiferFlags & static_cast<int>(ModifierType::Alt)) ss << "Alt + ";
			ss << KeyCodeToString(m_Key);
			return ss.str();
		}
	private:
		std::string m_Name = "Untitled";
		int m_ModiferFlags = 0;
		Key::KeyCode m_Key = Key::KeyCode::Escape; // TODO : Just one key
		std::function<void()> m_Action;
	};

	class ShortcutLibrary
	{
	public:
		ShortcutLibrary() = default;
		inline void addShortcut(const Shortcut& vShortcut) { m_Shortcuts[vShortcut.getActionName()] = vShortcut; }
	private:
		std::unordered_map<std::string, Shortcut> m_Shortcuts;
	};

}
