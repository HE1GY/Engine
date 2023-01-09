#pragma once

#include "Engine/Window.h"
#include "GLFW/glfw3.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine
{
class WindowsWindow : public Window
{
  public:
    WindowsWindow(const WindowsProps &props);

    virtual void OnUpdate() override;

    virtual void SetEventCallback(const EventCallbackFn &fun) override;
    void *GetNativeWindow() override;

  private:
    EventCallbackFn m_callback;
    GLFWwindow *m_native_window;
    GraphicContext *m_graphic_context;
};

} // namespace Engine
