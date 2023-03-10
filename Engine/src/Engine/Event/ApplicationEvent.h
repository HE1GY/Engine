#pragma once
#include "Event.h"

namespace Engine
{
	class WindowResized : public Event
	{
	public:
		WindowResized(int width, int height)
				:m_width(width), m_height(height)
		{
		}

		EVENT_TYPE(WindowResized)

		virtual std::string ToString() override
		{
			std::stringstream ss;
			ss << "WindowResized: " << "Width: " << m_width << " " << "Height: " << m_height;
			return ss.str();
		}

		inline int GetWidth() const
		{
			return m_width;
		}
		inline int GetHeight() const
		{
			return m_height;
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		int m_width, m_height;
	};

	class WindowClosed : public Event
	{
	public:
		EVENT_TYPE(WindowClosed)

		virtual std::string ToString() override
		{
			std::stringstream ss;
			ss << "WindowClosed";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}