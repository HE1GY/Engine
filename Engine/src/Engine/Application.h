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

    inline static Application* Get()
    {
        return s_instance;
    }

    inline std::shared_ptr<Window> GetWindow()
    {
        return m_window;
    }
  private:
    static Application* s_instance;

    bool OnWindowsClosed(WindowClosed &event);

    std::shared_ptr<Window> m_window;
    bool m_is_running{true};
    LayerStack m_layer_stack;
};
}