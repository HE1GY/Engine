#pragma once

namespace Engine
{
class Renderer
{
  public:
    enum class API
    {
        None,
        OpenGL,
    };

    inline static API get_renderer_API()
    {
        return s_render_API;
    }

  private:
    static API s_render_API;
};

} // namespace Engine