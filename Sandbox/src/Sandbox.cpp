#include "Engine.h"

class ExampleLayer : public Engine::Layer
{
  public:
    ExampleLayer() : Layer("ExampleLayer")
    {
    }

    void OnEvent(Engine::Event &event) override
    {
        Engine::EventDispatcher ed(event);
        std::function<bool(Engine::KeyPress &)> fn = std::bind(&ExampleLayer::OnKeyEvent, this, std::placeholders::_1);
        ed.Dispatch<Engine::KeyPress>(fn);
    }

    void OnUpdate() override
    {
        TRACE("Mouse pos: {0},{1}", Engine::Input::GetMousePos().first, Engine::Input::GetMousePos().second);
    }

  private:
    bool OnKeyEvent(Engine::KeyPress &event)
    {
        INFO(event.ToString());
        return false;
    }
};

class SandBox : public Engine::Application
{
  public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandBox() override = default;
};

Engine::Application *CreateApplication()
{
    return new SandBox();
}
