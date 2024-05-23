#pragma once
#include "pkpch.h"
#include "Pika/Core/Core.h"

namespace Pika {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonRleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)  inline static EventType getStaticType() { return EventType::##type; }\
								inline EventType getEventType() const override { return getStaticType(); }\
								inline const char* getName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) inline int getCategoryFlags() const override { return category; }

	class PIKA_API Event
	{
	public:
		virtual ~Event() = default;

		bool m_Handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		inline virtual std::string toString() const { return getName(); }
		inline bool isInCategory(EventCategory vCategory) const { return getCategoryFlags() & static_cast<int>(vCategory); }
	};

	class PIKA_API EventDispatcher //dispatch event to corresponding function 
	{
	public:
		EventDispatcher() = delete;
		EventDispatcher(Event& vEvent) :
			m_Event{ vEvent } {}

		template<typename T, typename F> //T:Event£¬F:function
		bool dispatch(const F& func) {
			if (m_Event.getEventType() == T::getStaticType()) {
				m_Event.m_Handled |= func(*static_cast<T*>(& m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

}