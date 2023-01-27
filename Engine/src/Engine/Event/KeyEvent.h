#pragma once

#include "Event.h"

namespace Engine
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(int key_code)
				:m_key_code(key_code)
		{
		}
		inline int get_key() const
		{
			return m_key_code;
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		int m_key_code;
	};

	class KeyPress : public KeyEvent
	{
	public:
		KeyPress(int key_code, int repeated_count)
				:KeyEvent(key_code), m_repeated_count(repeated_count)
		{
		}

		EVENT_TYPE(KeyPress)
		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "KeyPress: " << (char)m_key_code;
			return ss.str();
		}

	private:
		int m_repeated_count;
	};

	class KeyReleased : public KeyEvent
	{
	public:
		KeyReleased(int key_code)
				:KeyEvent(key_code)
		{
		}

		EVENT_TYPE(KeyReleased)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "KeyReleased: " << (char)m_key_code;
			return ss.str();
		}
	};

	class KeyTyped : public KeyEvent
	{
	public:
		KeyTyped(int key_code)
				:KeyEvent(key_code)
		{
		}

		EVENT_TYPE(KeyTyped)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "KeyTyped: " << (char)m_key_code;
			return ss.str();
		}
	};

} // namespace Engine