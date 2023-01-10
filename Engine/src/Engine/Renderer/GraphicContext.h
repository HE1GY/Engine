#pragma once

namespace Engine
{
class GraphicContext
{
  public:
    virtual void Init()=0;
    virtual void SwapBuffer()=0;
};
} // namespace Engine