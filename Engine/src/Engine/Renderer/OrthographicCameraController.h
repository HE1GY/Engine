#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/MouseEvent.h"

#include "Engine/Renderer/OrthographicCamera.h"

namespace Engine
{
class OrthographicCameraController
{
  public:
    OrthographicCameraController(float aspect_ratio, bool is_rotating);

    void OnUpdate(TimeStep ts);
    void OnEvent(Event &event);

    OrthographicCamera &get_camera()
    {
        return m_camera;
    }

  private:
    bool OnMouseScroll(MouseScrolled &event);
    bool OnWindowResized(WindowResized &event);

  private:
    float m_aspect_ratio;
    bool m_is_rotating;
    float m_zoom_level{1};
    OrthographicCamera m_camera;
};
} // namespace Engine