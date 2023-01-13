#pragma once

#include <functional>
#include <string>
#include <sstream>

#define BIND(x) std::bind(x, this,std::placeholders::_1);

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

	// event categories for filtering

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual std::string ToString() = 0;

		bool Handled = false;
	};

#define EVENT_TYPE(type) virtual EventType GetEventType()const override{return EventType::type;}\
                         static EventType GetStaticEventType() {return  EventType::type;}

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
		bool Dispatch(EventFn<T>& fn)
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