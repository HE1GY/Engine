#include "WindowsWindow.h"
#include <iostream>

#include "Engine/Core.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Event/MouseEvent.h"

namespace Engine
{
WindowsWindow::WindowsWindow(const WindowsProps &props)
{
    int valid = glfwInit();
    ASSERT(valid);

    m_native_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
    if (!m_native_window)
    {
        glfwTerminate();
        ASSERT(0);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_native_window);

    glfwSetWindowUserPointer(m_native_window, &m_callback);

    // Event handling
    glfwSetKeyCallback(m_native_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        EventCallbackFn *event_callback = (EventCallbackFn *)glfwGetWindowUserPointer(window);

        if (action == GLFW_PRESS)
        {
            KeyPress key_press(key, 0);
            (*event_callback)(key_press);
        }
        else if (action == GLFW_RELEASE)
        {
            KeyReleased key_released(key);
            (*event_callback)(key_released);
        }
        else if (action == GLFW_REPEAT)
        {
            KeyPress key_press(key, 1);
            (*event_callback)(key_press);
        }
    });
    glfwSetCharCallback(m_native_window, [](GLFWwindow *window, unsigned int codepoint) {
        EventCallbackFn *event_callback = (EventCallbackFn *)glfwGetWindowUserPointer(window);
        KeyTyped key_typed(codepoint);
        (*event_callback)(key_typed);
    });

    glfwSetWindowCloseCallback(m_native_window, [](GLFWwindow *window) {
        EventCallbackFn *event_callback = (EventCallbackFn *)glfwGetWindowUserPointer(window);
        WindowClosed close_event;
        (*event_callback)(close_event);
    });
}

Window *Window::Create(const Window::WindowsProps &props)
{
    return new WindowsWindow(props);
}

void WindowsWindow::OnUpdate()
{
    glfwSwapBuffers(m_native_window);

    /* Poll for and process events */
    glfwPollEvents();
}
void WindowsWindow::SetEventCallback(const EventCallbackFn &fun)
{
    m_callback = fun;
}
void *WindowsWindow::GetNativeWindow()
{
    return m_native_window;
}
} // namespace Engine