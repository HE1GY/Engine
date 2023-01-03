#pragma once


namespace Engine
{
class Input
{
  public:
    static inline bool IsMouseButtonPress(int button)
    {
        return s_instance->IsMouseButtonPressImpl(button);
    }
    static inline bool IsKeyPress(int key_code)
    {
        return s_instance->IsKeyPressImpl(key_code);
    }

    static inline std::pair<float, float> GetMousePos()
    {
        return s_instance->GetMousePosImpl();
    }

  protected:
    static Input *s_instance;

    virtual std::pair<float, float> GetMousePosImpl() = 0;
    virtual bool IsMouseButtonPressImpl(int button) = 0;
    virtual bool IsKeyPressImpl(int key_code) = 0;
};
} // namespace Engine