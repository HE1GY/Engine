#pragma once

#include <functional>
#include <string>
#include <sstream>

#define BIND_EVENT_FUNC(x) std::bind(&x, this,std::placeholders::_1)

namespace Engine
{
	// all types of event for comparing and invoking appropriate func
	enum class EventType
	{
		None,
		WindowResized,
		WindowClosed,
		KeyPress,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4,
	};

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() = 0;

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		bool Handled = false;
	};

#define EVENT_TYPE(type) virtual EventType GetEventType()const override{return EventType::type;}\
                         static EventType GetStaticEventType() {return  EventType::type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class EventDispatcher
	{
	public:
		template<typename T>
		using EventFn = std::function<bool(T&)>;

		EventDispatcher(Event& e)
				:m_event(e)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> fn)
		{
			if (m_event.GetEventType() == T::GetStaticEventType())
			{
				m_event.Handled = fn(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};
}