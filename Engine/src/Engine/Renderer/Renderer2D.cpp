#include <glm/ext/matrix_transform.hpp>
#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Engine
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		float texture_slot;
	};

	struct Renderer2DData
	{
		const uint32_t k_max_quads = 10000;
		const uint32_t k_max_vertices = k_max_quads * 4;
		const uint32_t k_max_indices = k_max_quads * 6;
		static const uint32_t k_max_texture_slot = 32;//TODO renderer prop

		Ref<Shader> default_shader;
		Ref<Texture> default_texture;
		Ref<VertexArray> default_vao;
		Ref<VertexBuffer> default_vb;

		uint32_t quad_index_count{ 0 };
		QuadVertex* quad_vertex_buffer_base{ nullptr };
		QuadVertex* quad_vertex_buffer_ptr{ nullptr };

		std::array<Ref<Texture>, k_max_texture_slot> textures{ 0 };
		uint32_t texture_index{ 1 };

		std::array<glm::vec4, 4> quad_vertices;
	};

	Renderer2DData Renderer2D::s_data;

	void Engine::Renderer2D::Init()
	{
		PROFILER_FUNCTION();

		s_data.default_shader = Shader::Create("../../../Sandbox/assets/shaders/default_2D_shader.glsl");
		s_data.default_shader->Bind();

		s_data.default_texture = Texture2D::Create(1, 1);
		uint32_t tex_data = 0xffffffff;
		s_data.default_texture->SetData(&tex_data, sizeof(tex_data));
		s_data.default_texture->Bind();

		s_data.default_vao = VertexArray::Create();
		s_data.default_vao->Bind();

		s_data.default_vb = Engine::VertexBuffer::Create(s_data.k_max_vertices * sizeof(QuadVertex));
		s_data.default_vb->Bind();
		s_data.default_vb->set_layout({
				{ "a_position", Engine::ShaderDataType::Float3, false },
				{ "a_color", Engine::ShaderDataType::Float4, false },
				{ "a_tex_coord", Engine::ShaderDataType::Float2, false },
				{ "a_tex_slot", Engine::ShaderDataType::Float, false }
		});
		s_data.default_vao->AddVertexBuffer(s_data.default_vb);

		s_data.quad_vertex_buffer_base = new QuadVertex[s_data.k_max_vertices];

		uint32_t* quad_indices = new uint32_t[s_data.k_max_indices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.k_max_indices; i += 6)
		{
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;

			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;

			offset += 4;
		}

		Engine::Ref<Engine::IndexBuffer> vo_index_box = Engine::IndexBuffer::Create(quad_indices, s_data.k_max_indices);
		vo_index_box->Bind();
		s_data.default_vao->SetIndexBuffer(vo_index_box);
		delete[] quad_indices;

		s_data.textures[0] = s_data.default_texture;

		int sampler[s_data.k_max_texture_slot];
		for (int i = 0; i < s_data.k_max_texture_slot; ++i)
		{
			sampler[i] = i;
		}
		s_data.default_shader->SetIntArray("u_texture2D", sampler, s_data.k_max_texture_slot);

		s_data.quad_vertices[0] = { -0.5f, -0.5f, 0, 1 };
		s_data.quad_vertices[1] = { 0.5f, -0.5f, 0, 1 };
		s_data.quad_vertices[2] = { 0.5f, 0.5f, 0, 1 };
		s_data.quad_vertices[3] = { -0.5f, 0.5f, 0, 1 };

	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		PROFILER_FUNCTION();

		s_data.default_shader->Bind();
		s_data.default_shader->SetMat4("u_view_projection", camera.get_view_projection_matrix());

		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

		s_data.quad_index_count = 0;
	}
	void Renderer2D::EndScene()
	{
		PROFILER_FUNCTION();

		s_data.default_vb->SetData(s_data.quad_vertex_buffer_base, s_data.quad_index_count * sizeof(QuadVertex));
		Flush();
	}
	void Renderer2D::Flush()
	{
		for (int i = 0; i < s_data.texture_index; ++i)
		{
			s_data.textures[i]->Bind(i);
		}

		RendererCommand::DrawIndex(s_data.default_vao, s_data.quad_index_count);
	}

	void Engine::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0 }, scale, color);
	}
	void Engine::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();
		const float texture_slot{ 0.0f };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[0];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[1];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[2];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[3];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;

		/*s_data.default_texture->Bind(0);
		s_data.default_vao->Bind();
		s_data.default_shader->SetMat4("u_model",
				glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 }));

		RendererCommand::DrawIndex(s_data.default_vao);*/
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const float rotation,
			const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		const float texture_slot{ 0.0f };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[0];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[1];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[2];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[3];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;

		/*s_data.default_texture->Bind(0);
		s_data.default_vao->Bind();

		s_data.default_shader->SetMat4("u_model",
				glm::translate(glm::mat4(1.0f), position)
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
						* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 }));

		s_data.default_shader->SetVec4("u_color", color);

		RendererCommand::DrawIndex(s_data.default_vao);*/
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

		float texture_slot{ 0.0f };
		for (int i = 1; i < s_data.texture_index; ++i)
		{
			if (texture->get_renderer_id() == s_data.textures[i]->get_renderer_id())
			{
				texture_slot = i;
			}
		}

		if (texture_slot == 0.0f)
		{
			texture_slot = (float)s_data.texture_index;
			s_data.textures[texture_slot] = texture;
			s_data.texture_index++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[0];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[1];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 0 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[2];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 1, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[3];
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->tex_coord = { 0, 1 };
		s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;

		/*s_data.default_vao->Bind();

		s_data.default_shader->SetMat4("u_model",
				glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 }));
		texture->Bind(0);

		s_data.default_shader->SetVec4("u_color", color);

		RendererCommand::DrawIndex(s_data.default_vao);*/
	}

}