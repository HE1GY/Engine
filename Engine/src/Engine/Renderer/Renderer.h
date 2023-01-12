#pragma once

#include "OrthographicCamera.h"
#include "RendererCommand.h"
#include "Shader.h"

namespace Engine
{
class Renderer
{
  public:
    static void BeginScene(OrthographicCamera &camera);

    static void EndScene();

    static void Submit(const std::shared_ptr<Shader> &shader, std::shared_ptr<VertexArray> vertex_array);

    inline static RendererAPI::API get_renderer_API()
    {
        return RendererAPI::get_renderer_API();
    }

  private:
    struct SceneData{
        glm::mat4 view_projection_matrix;
    };

    static SceneData* s_scene_data;
};

} // namespace Engine