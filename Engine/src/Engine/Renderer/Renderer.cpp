#include "pch.h"

#include "Engine/Core.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{

	Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData;
	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_scene_data->view_projection_matrix = camera.get_view_projection_matrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, Ref<VertexArray> vertex_array, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform("u_view_project",
				s_scene_data->view_projection_matrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniform("u_model", transform);
		vertex_array->Bind();
		RendererCommand::DrawIndex(vertex_array);
	}
	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

} // namespace Engine