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
	protected:
		int m_key_code;
	};

	class KeyPress : public KeyEvent
	{
	public:
		KeyPress(int key_code, int repeted_cout)
				:KeyEvent(key_code), m_repeted_cout(repeted_cout)
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
		int m_repeted_cout;
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

}