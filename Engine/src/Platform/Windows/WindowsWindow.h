#pragma once

#include "Engine/Window.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowsProps& props);

		virtual void OnUpdate() override;

		virtual void SetEventCallback(const EventCallbackFn& fun) override;
		void* GetNativeWindow() override;

		virtual void SetVSync(bool on) override;
		inline virtual bool ISVSync() const override
		{
			return m_is_vsync;
		};

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
