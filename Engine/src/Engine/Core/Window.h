#pragma once

#include "Engine/Event/Event.h"

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

		virtual ~Window() = default;
		static Window* Create(const WindowsProps& props = WindowsProps());

		virtual void OnUpdate() = 0;

		virtual void set_event_callback(const EventCallbackFn& fun) = 0;

		virtual void* get_native_window() = 0;

		virtual void set_VSync(bool on) = 0;
		virtual bool get_VSync() const = 0;

		virtual float get_current_time() = 0;

		virtual unsigned int get_width() = 0;
		virtual unsigned int get_height() = 0;
	};
} // namespace Engine
