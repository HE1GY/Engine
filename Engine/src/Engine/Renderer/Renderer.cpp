#include "Renderer.h"
#include "pch.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{

Renderer::SceneData *Renderer::s_scene_data = new Renderer::SceneData;
void Renderer::BeginScene(OrthographicCamera &camera)
{
    s_scene_data->view_projection_matrix = camera.get_view_projection_matrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, std::shared_ptr<VertexArray> vertex_array,
                      const glm::mat4 &transform)
{
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform("u_view_project",
                                                                   s_scene_data->view_projection_matrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform("u_model", transform);
    vertex_array->Bind();
    RendererCommand::DrawIndex(vertex_array);
}

} // namespace Engine