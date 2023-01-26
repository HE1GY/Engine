#pragma once

#include "Engine/Core/Window.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowsProps& props);

		virtual void OnUpdate() override;

		virtual void set_event_callback(const EventCallbackFn& fun) override;
		void* get_native_window() override;

		virtual void set_VSync(bool on) override;
		inline virtual bool get_VSync() const override
		{
			return m_is_vsync;
		};

		virtual float get_current_time() override;

		inline virtual unsigned int get_width() override
		{
			return m_props.width;
		};
		inline virtual unsigned int get_height() override
		{
			return m_props.height;
		};

	private:
		EventCallbackFn m_callback;
		GLFWwindow* m_native_window;
		GraphicContext* m_graphic_context;
		bool m_is_vsync;
		WindowsProps m_props;
	};

} // namespace Engine
