#pragma once
#include "Engine/Renderer/GraphicContext.h"

struct GLFWwindow;
namespace Engine
{

class OpenGLContext : public GraphicContext
{
  public:
    OpenGLContext(GLFWwindow *window_handler);
    virtual void Init() override;
    virtual void SwapBuffer() override;

  private:
    GLFWwindow *m_window_handler;
};
} // namespace Engine
