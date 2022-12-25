#pragma once

#include <string>

#include "Event/Event.h"

namespace Engine
{
	class Window
	{
	public:
		struct WindowsProps
		{
			WindowsProps(unsigned int width = 1280, unsigned int height = 720, std::string title = "Engine")
					:width{ width }, height{ height }, title{ title }
			{
			}
			unsigned int width, height;
			std::string title;
		};

		using EventCallbackFn = std::function<void(Event&)>;

		static Window* Create(const WindowsProps& props = WindowsProps());

		virtual void OnUpdate() = 0;

		virtual void SetEventCallback(const EventCallbackFn& fun) = 0;
	};
}
