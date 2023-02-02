#include "pch.h"

#include "WindowsWindow.h"

#include "Engine/Core/Core.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Event/MouseEvent.h"

namespace Engine
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowsProps& props)
			:m_window_data{ props }
	{
		PROFILER_FUNCTION();

		{
			PROFILER_SCOPE("glfwInit");
			int valid = glfwInit();
			ASSERT(valid);
		}

		{
			PROFILER_SCOPE("glfwCreateWindow");
			m_native_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
			if (!m_native_window)
			{
				glfwTerminate();
				ASSERT(0);
			}
		}
		glfwSetErrorCallback(GLFWErrorCallback);

		m_graphic_context = new OpenGLContext(m_native_window);
		m_graphic_context->Init();

		glfwSetWindowUserPointer(m_native_window, &m_window_data);

		// Event handling
		glfwSetKeyCallback(m_native_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

		  if (action == GLFW_PRESS)
		  {
			  KeyPress key_press(key, 0);
			  (data->callback)(key_press);
		  }
		  else if (action == GLFW_RELEASE)
		  {
			  KeyReleased key_released(key);
			  (data->callback)(key_released);
		  }
		  else if (action == GLFW_REPEAT)
		  {
			  KeyPress key_press(key, 1);
			  (data->callback)(key_press);
		  }
		});
		glfwSetCharCallback(m_native_window, [](GLFWwindow* window, unsigned int codepoint)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		  KeyTyped key_typed(codepoint);
		  (data->callback)(key_typed);
		});

		glfwSetWindowCloseCallback(m_native_window, [](GLFWwindow* window)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		  WindowClosed close_event;
		  (data->callback)(close_event);
		});

		glfwSetWindowSizeCallback(m_native_window, [](GLFWwindow* window, int width, int height)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

		  data->props.width = width;
		  data->props.height = height;

		  WindowResized e(width, height);
		  (data->callback)(e);
		});

		glfwSetMouseButtonCallback(m_native_window, [](GLFWwindow* window, int button, int action, int mods)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

		  if (action == GLFW_PRESS)
		  {
			  MouseButtonPressed key_press(button);
			  (data->callback)(key_press);
		  }
		  else if (action == GLFW_RELEASE)
		  {
			  MouseButtonReleased key_released(button);
			  (data->callback)(key_released);
		  }
		});

		glfwSetScrollCallback(m_native_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		  MouseScrolled e(xoffset, yoffset);
		  (data->callback)(e);
		});

		glfwSetCursorPosCallback(m_native_window, [](GLFWwindow* window, double xpos, double ypos)
		{
		  WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		  MouseMoved e(xpos, ypos);
		  (data->callback)(e);
		});
	}

	Window* Window::Create(const Window::WindowsProps& props)
	{
		return new WindowsWindow(props);
	}

	void WindowsWindow::OnUpdate()
	{
		PROFILER_FUNCTION();
		glfwPollEvents();
		m_graphic_context->SwapBuffer();
	}
	void WindowsWindow::set_event_callback(const EventCallbackFn& fun)
	{
		m_window_data.callback = fun;
	}
	void* WindowsWindow::get_native_window()
	{
		return m_native_window;
	}

	void WindowsWindow::set_VSync(bool on)
	{
		PROFILER_FUNCTION();

		m_is_vsync = on;
		glfwSwapInterval(m_is_vsync ? 1 : 0);
	}
	float WindowsWindow::get_current_time()
	{
		return (float)glfwGetTime();
	}
} // namespace Engine