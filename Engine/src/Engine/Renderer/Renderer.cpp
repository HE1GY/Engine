#include "pch.h"

#include "Engine/Core/Core.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace Engine
{

	Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData;
	void Renderer::Init()
	{
		PROFILER_FUNCTION();

		RendererCommand::Init();
		Renderer2D::Init();
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
		PROFILER_FUNCTION();

		shader->Bind();
		shader->SetMat4("u_view_project", s_scene_data->view_projection_matrix);
		shader->SetMat4("u_model", transform);
		vertex_array->Bind();
		RendererCommand::DrawIndex(vertex_array);
	}
	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		PROFILER_FUNCTION();

		RendererCommand::SetViewport(0, 0, width, height);
	}
	void Renderer::ShutDown()
	{
		PROFILER_FUNCTION();

		Renderer2D::ShutDown();
	}

} // namespace Engine