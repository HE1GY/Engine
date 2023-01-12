#include "Renderer.h"
#include "pch.h"

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

void Renderer::Submit(const std::shared_ptr<Shader> &shader, std::shared_ptr<VertexArray> vertex_array)
{
    shader->Bind();
    shader->UploadUniform("u_vp", s_scene_data->view_projection_matrix);
    vertex_array->Bind();
    RendererCommand::DrawIndex(vertex_array);
}

} // namespace Engine