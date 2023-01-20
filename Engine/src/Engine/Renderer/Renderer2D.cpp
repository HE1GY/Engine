#include <glm/ext/matrix_transform.hpp>
#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Engine
{

	Renderer2DData* Renderer2D::s_data = new Renderer2DData();

	void Engine::Renderer2D::Init()
	{
		PROFILER_FUNCTION();

		s_data->default_shader = Shader::Create("../../Sandbox/assets/shaders/default_2D_shader.glsl");

		s_data->default_texture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_data->default_texture->SetData(&data, sizeof(data));

		s_data->default_vao = VertexArray::Create();
		s_data->default_vao->Bind();
		float pos_box[] = {
				-0.5, -0.5, 0, 0, 0,
				0.5, -0.5, 0, 1, 0,
				0.5, 0.5, 0, 1, 1,
				-0.5, 0.5, 0, 0, 1,
		};
		Engine::Ref<Engine::VertexBuffer> vo_data_box;
		vo_data_box = Engine::VertexBuffer::Create(pos_box, sizeof(pos_box));
		vo_data_box->Bind();

		vo_data_box->set_layout({
				{ "a_position", Engine::ShaderDataType::Float3, false },
				{ "a_tex_coord", Engine::ShaderDataType::Float2, false }
		});

		uint32_t elements_box[] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ref<Engine::IndexBuffer> vo_index_box;
		vo_index_box = Engine::IndexBuffer::Create(elements_box, sizeof(elements_box) / sizeof(uint32_t));
		vo_index_box->Bind();

		s_data->default_vao->AddVertexBuffer(vo_data_box);
		s_data->default_vao->set_indexBuffer(vo_index_box);
	}

	void Renderer2D::ShutDown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		PROFILER_FUNCTION();

		s_data->default_shader->Bind();
		s_data->default_shader->SetMat4("u_view_projection", camera.get_view_projection_matrix());
	}
	void Renderer2D::EndScene()
	{
		PROFILER_FUNCTION();
	}
	void Engine::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0 }, scale, color);
	}
	void Engine::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		s_data->default_texture->Bind(0);
		s_data->default_vao->Bind();

		s_data->default_shader->SetMat4("u_model",
				glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 }));

		s_data->default_shader->SetVec4("u_color", color);

		RendererCommand::DrawIndex(s_data->default_vao);
	}

	void Engine::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale,
			const Engine::Ref<Engine::Texture>& texture, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0 }, scale, texture, color);
	}
	void Engine::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale,
			const Engine::Ref<Engine::Texture>& texture, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		s_data->default_vao->Bind();

		s_data->default_shader->SetMat4("u_model",
				glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 }));
		texture->Bind(0);

		s_data->default_shader->SetVec4("u_color", color);

		RendererCommand::DrawIndex(s_data->default_vao);
	}
}