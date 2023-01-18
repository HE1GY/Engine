#pragma once
#include "Engine/Core/Layer.h"

#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Event/MouseEvent.h"

namespace Engine
{
class ImGuiLayer : public Layer
{
  public:
    ImGuiLayer() : Layer("ImGuiLayer")
    {
    }
    virtual ~ImGuiLayer() override = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void Begin();
    void End();
};
} // namespace Engine