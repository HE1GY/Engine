#pragma once

#include "Engine/Event/Event.h"

namespace Engine
{
	class Window
	{
	public:
		struct WindowsProps
		{
			WindowsProps(unsigned int width = 1600, unsigned int height = 900, std::string title = "Engine")
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

		virtual void* GetNativeWindow() = 0;

		virtual void set_VSync(bool on) = 0;
		virtual bool get_VSync() const = 0;

		virtual float get_current_time() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
	};
} // namespace Engine
