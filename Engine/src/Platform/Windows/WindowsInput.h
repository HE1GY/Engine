#pragma once

#include "Engine/Core/Input.h"

namespace Engine
{
class WindowsInput : public Input
{
  protected:
    std::pair<float, float> GetMousePosImpl() override;
    bool IsMouseButtonPressImpl(int button) override;
    bool IsKeyPressImpl(int key_code) override;
};
} // namespace Engine