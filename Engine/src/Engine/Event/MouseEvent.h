#pragma once
#include "Event.h"

namespace Engine
{
	class MouseMoved : public Event
	{
	public:
		MouseMoved(int x, int y)
				:m_x(x), m_y(y)
		{
		}

		EVENT_TYPE(MouseMoved)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseMoved: " << "X: " << m_x << " " << "Y: " << m_y;
			return ss.str();
		}

	private:
		int m_x, m_y;
	};

	class MouseButtonPressed : public Event
	{
	public:
		MouseButtonPressed(int key_code)
				:m_key_code(key_code)
		{
		}

		EVENT_TYPE(MouseButtonPressed)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_key_code;
			return ss.str();
		}

	private:
		int m_key_code;
	};

	class MouseButtonReleased : public Event
	{
	public:
		MouseButtonReleased(int key_code)
				:m_key_code(key_code)
		{
		}

		EVENT_TYPE(MouseButtonReleased)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << m_key_code;
			return ss.str();
		}

	private:
		int m_key_code;
	};

	class MouseScrolled : public Event
	{
	public:
		MouseScrolled(int x_offset, int y_offset)
				:m_x_offset(x_offset), m_y_offset(y_offset)
		{
		}

		EVENT_TYPE(MouseScrolled)

		virtual std::string ToString()
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << "X_offset: " << m_x_offset << " " << "Y_offset: " << m_y_offset;
			return ss.str();
		}

	private:
		int m_x_offset, m_y_offset;
	};

}