#pragma once

#include <memory>

#include "Engine/Event/ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"

namespace Engine
{
class Application
{
  public:
    Application();
    virtual ~Application() = default;
    void Run();
    void OnEvent(Event &event);

    void PushLayer(Layer* layer);
    void PushOverLay(Layer* layer);
    void EraseLayer(Layer* layer);
  private:
    bool OnWindowsClosed(WindowClosed &event);

    std::unique_ptr<Window> m_window;
    bool m_is_running{true};
    LayerStack m_layer_stack;
};
}