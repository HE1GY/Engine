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

		//for editor
		int32_t entity_id{ 0 };
	};

	struct CircleVertex
	{
		glm::vec3 world_position;
		glm::vec3 local_position;
		glm::vec4 color;
		float thickness;
		float fade;
		int32_t entity_id{ 0 };
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		int32_t entity_id{ 0 };
	};

	struct Renderer2DData
	{
		const uint32_t k_max_quads = 10000;
		const uint32_t k_max_vertices = k_max_quads * 4;
		const uint32_t k_max_indices = k_max_quads * 6;
		static const uint32_t k_max_texture_slot = 32;    //TODO renderer prop

		Renderer2D::Statistics stats;

		Ref<Shader> quad_shader;
		Ref<Texture> white_texture;
		Ref<VertexArray> quad_vao;
		Ref<VertexBuffer> quad_vb;

		uint32_t quad_index_count{ 0 };
		QuadVertex* quad_vertex_buffer_base{ nullptr };
		QuadVertex* quad_vertex_buffer_ptr{ nullptr };

		std::array<Ref<Texture>, k_max_texture_slot> textures;
		uint32_t texture_index{ 1 };

		std::array<glm::vec4, 4> quad_vertices;

		//Circle
		Ref<Shader> circle_shader;
		Ref<VertexArray> circle_vao;
		Ref<VertexBuffer> circle_vb;

		uint32_t circle_index_count{ 0 };
		CircleVertex* circle_vertex_buffer_base{ nullptr };
		CircleVertex* circle_vertex_buffer_ptr{ nullptr };

		//Line
		Ref<Shader> line_shader;
		Ref<VertexArray> line_vao;
		Ref<VertexBuffer> line_vb;

		uint32_t line_index_count{ 0 };
		LineVertex* line_vertex_buffer_base{ nullptr };
		LineVertex* line_vertex_buffer_ptr{ nullptr };

	};

	Renderer2DData Renderer2D::s_data;

	void Engine::Renderer2D::Init()
	{
		PROFILER_FUNCTION();

		s_data.quad_shader = Shader::Create("../../../Sandbox/assets/shaders/default_2D_shader.glsl");

		s_data.white_texture = Texture2D::Create(1, 1);
		uint32_t tex_data = 0xffffffff;
		s_data.white_texture->SetData(&tex_data, sizeof(tex_data));
		s_data.textures[0] = s_data.white_texture;

		s_data.quad_vao = VertexArray::Create();
		s_data.quad_vao->Bind();
		{
			s_data.quad_vb = VertexBuffer::Create(s_data.k_max_vertices * sizeof(QuadVertex));
			s_data.quad_vb->Bind();
			s_data.quad_vb->set_layout({
					{ "a_position", Engine::ShaderDataType::Float3, false },
					{ "a_color", Engine::ShaderDataType::Float4, false },
					{ "a_tex_coord", Engine::ShaderDataType::Float2, false },
					{ "a_tex_slot", Engine::ShaderDataType::Float, false },
					{ "a_entity_id", Engine::ShaderDataType::Int }
			});
			s_data.quad_vao->AddVertexBuffer(s_data.quad_vb);

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

			Ref<IndexBuffer> ib_box = IndexBuffer::Create(quad_indices, s_data.k_max_indices);
			ib_box->Bind();
			s_data.quad_vao->SetIndexBuffer(ib_box);
			delete[] quad_indices;
		}

		int sampler[s_data.k_max_texture_slot];
		for (int i = 0; i < s_data.k_max_texture_slot; ++i)
		{
			sampler[i] = i;
		}
		s_data.quad_shader->Bind();
		s_data.quad_shader->SetIntArray("u_texture2D", sampler, s_data.k_max_texture_slot);

		s_data.quad_vertices[0] = { -0.5f, -0.5f, 0, 1 };
		s_data.quad_vertices[1] = { 0.5f, -0.5f, 0, 1 };
		s_data.quad_vertices[2] = { 0.5f, 0.5f, 0, 1 };
		s_data.quad_vertices[3] = { -0.5f, 0.5f, 0, 1 };

		//Circle

		s_data.circle_shader = Shader::Create("../../../Sandbox/assets/shaders/default_2D_circle_shader.glsl");

		s_data.circle_vao = VertexArray::Create();
		{
			s_data.circle_vao->Bind();

			s_data.circle_vb = VertexBuffer::Create(s_data.k_max_vertices * sizeof(CircleVertex));
			s_data.circle_vb->Bind();
			s_data.circle_vb->set_layout({
					{ "a_position", Engine::ShaderDataType::Float3 },
					{ "a_local", Engine::ShaderDataType::Float3 },
					{ "a_color", Engine::ShaderDataType::Float4 },
					{ "a_thickness", Engine::ShaderDataType::Float },
					{ "a_fade", Engine::ShaderDataType::Float },
					{ "a_entity_id", Engine::ShaderDataType::Int }
			});
			s_data.circle_vao->AddVertexBuffer(s_data.circle_vb);

			s_data.circle_vertex_buffer_base = new CircleVertex[s_data.k_max_vertices];

			uint32_t* circle_indices = new uint32_t[s_data.k_max_indices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_data.k_max_indices; i += 6)
			{
				circle_indices[i + 0] = offset + 0;
				circle_indices[i + 1] = offset + 1;
				circle_indices[i + 2] = offset + 2;

				circle_indices[i + 3] = offset + 2;
				circle_indices[i + 4] = offset + 3;
				circle_indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> ib_circle = IndexBuffer::Create(circle_indices, s_data.k_max_indices);
			ib_circle->Bind();
			s_data.circle_vao->SetIndexBuffer(ib_circle);
			delete[] circle_indices;
		}

		//Line
		s_data.line_shader = Shader::Create("../../../Sandbox/assets/shaders/default_2D_line_shader.glsl");

		s_data.line_vao = VertexArray::Create();
		{
			s_data.line_vao->Bind();

			s_data.line_vb = VertexBuffer::Create(s_data.k_max_vertices * sizeof(LineVertex));
			s_data.line_vb->Bind();
			s_data.line_vb->set_layout({
					{ "a_position", Engine::ShaderDataType::Float3 },
					{ "a_color", Engine::ShaderDataType::Float4 },
					{ "a_entity_id", Engine::ShaderDataType::Int }
			});
			s_data.line_vao->AddVertexBuffer(s_data.line_vb);

			s_data.line_vertex_buffer_base = new LineVertex[s_data.k_max_vertices];

			uint32_t* line_indices = new uint32_t[s_data.k_max_indices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_data.k_max_indices; i += 6)
			{
				line_indices[i + 0] = offset + 0;
				line_indices[i + 1] = offset + 1;
				line_indices[i + 2] = offset + 2;

				line_indices[i + 3] = offset + 2;
				line_indices[i + 4] = offset + 3;
				line_indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> ib_line = IndexBuffer::Create(line_indices, s_data.k_max_indices);
			ib_line->Bind();
			s_data.line_vao->SetIndexBuffer(ib_line);
			delete[] line_indices;
		}

	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::BeginScene(Camera& camera, const glm::mat4& transform)
	{
		PROFILER_FUNCTION();

		glm::mat4 view_proj = camera.GetProjection() * glm::inverse(transform);

		s_data.quad_shader->Bind();
		s_data.quad_shader->SetMat4("u_view_projection", view_proj);

		s_data.circle_shader->Bind();
		s_data.circle_shader->SetMat4("u_view_projection", view_proj);

		s_data.line_shader->Bind();
		s_data.line_shader->SetMat4("u_view_projection", view_proj);

		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
		s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

		s_data.quad_index_count = 0;
		s_data.circle_index_count = 0;
		s_data.line_index_count = 0;

		s_data.texture_index = 1;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		PROFILER_FUNCTION();

		s_data.quad_shader->Bind();
		s_data.quad_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

		s_data.circle_shader->Bind();
		s_data.circle_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

		s_data.line_shader->Bind();
		s_data.line_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
		s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

		s_data.quad_index_count = 0;
		s_data.circle_index_count = 0;
		s_data.line_index_count = 0;

		s_data.texture_index = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		PROFILER_FUNCTION();

		s_data.quad_shader->Bind();
		s_data.quad_shader->SetMat4("u_view_projection", camera.GetViewProjection());

		s_data.circle_shader->Bind();
		s_data.circle_shader->SetMat4("u_view_projection", camera.GetViewProjection());

		s_data.line_shader->Bind();
		s_data.line_shader->SetMat4("u_view_projection", camera.GetViewProjection());

		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
		s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

		s_data.quad_index_count = 0;
		s_data.circle_index_count = 0;
		s_data.line_index_count = 0;

		s_data.texture_index = 1;
	}

	void Renderer2D::EndScene()
	{
		PROFILER_FUNCTION();

		uint32_t size = (s_data.quad_vertex_buffer_ptr - s_data.quad_vertex_buffer_base) * sizeof(QuadVertex);
		s_data.quad_vb->SetData(s_data.quad_vertex_buffer_base, size);

		uint32_t size_circle =
				(s_data.circle_vertex_buffer_ptr - s_data.circle_vertex_buffer_base) * sizeof(CircleVertex);
		s_data.circle_vb->SetData(s_data.circle_vertex_buffer_base, size_circle);

		uint32_t size_line =
				(s_data.line_vertex_buffer_ptr - s_data.line_vertex_buffer_base) * sizeof(LineVertex);
		s_data.line_vb->SetData(s_data.line_vertex_buffer_base, size_line);

		Flush();
	}
	void Renderer2D::Flush()
	{
		for (int i = 0; i < s_data.texture_index; ++i)
		{
			s_data.textures[i]->Bind(i);
		}

		if (s_data.quad_index_count)
		{
			s_data.quad_shader->Bind();
			RendererCommand::DrawIndex(s_data.quad_vao, s_data.quad_index_count);
		}

		if (s_data.circle_index_count)
		{
			s_data.circle_shader->Bind();
			RendererCommand::DrawIndex(s_data.circle_vao, s_data.circle_index_count);
		}

		if (s_data.line_index_count)
		{
			s_data.line_shader->Bind();
			RendererCommand::DrawLine(s_data.line_vao, s_data.line_index_count);
		}

		s_data.stats.draw_calls++;
	}

	void Renderer2D::FlushAndReset()
	{
		PROFILER_FUNCTION();

		EndScene();

		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
		s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

		s_data.quad_index_count = 0;
		s_data.circle_index_count = 0;
		s_data.line_index_count = 0;

		s_data.texture_index = 1;
	}

	void Engine::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		DrawQuad({ position.x, position.y, 0 }, scale, color);
	}
	void Engine::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		DrawQuad(transform, color);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const float rotation_rad,
			const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		DrawQuad(transform, color);

	}

	//texture
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

		if (s_data.quad_index_count >= s_data.k_max_indices)
			FlushAndReset();


		//texture batch
		float texture_slot{ 0.0f };
		for (int i = 1; i < s_data.texture_index; ++i)
		{
			if (texture->GetRendererId() == s_data.textures[i]->GetRendererId())
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

		for (int i = 0; i < 4; ++i)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[i];
			s_data.quad_vertex_buffer_ptr->color = color;
			s_data.quad_vertex_buffer_ptr->tex_coord = { (i > 0 && i < 3) ? 1 : 0, i > 1 ? 1 : 0 };
			s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
			s_data.quad_vertex_buffer_ptr->entity_id = 0;
			s_data.quad_vertex_buffer_ptr++;
		}

		s_data.quad_index_count += 6;

		s_data.stats.quads++;

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<SubTexture2D>& sub_texture,
			const glm::vec4& color)
	{
		PROFILER_FUNCTION();
		DrawQuad({ position.x, position.y, 0 }, scale, sub_texture, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SubTexture2D>& sub_texture,
			const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		if (s_data.quad_index_count >= s_data.k_max_indices)
			FlushAndReset();

		//texture batch
		float texture_slot{ 0.0f };
		for (int i = 1; i < s_data.texture_index; ++i)
		{
			if (sub_texture->get_texture()->GetRendererId() == s_data.textures[i]->GetRendererId())
			{
				texture_slot = i;
			}
		}
		if (texture_slot == 0.0f)
		{
			texture_slot = (float)s_data.texture_index;
			s_data.textures[texture_slot] = sub_texture->get_texture();
			s_data.texture_index++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

		for (int i = 0; i < 4; ++i)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[i];
			s_data.quad_vertex_buffer_ptr->color = color;
			s_data.quad_vertex_buffer_ptr->tex_coord = sub_texture->get_coords()[i];
			s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
			s_data.quad_vertex_buffer_ptr->entity_id = 0;
			s_data.quad_vertex_buffer_ptr++;
		}

		s_data.quad_index_count += 6;

		s_data.stats.quads++;

	}
	void Renderer2D::DrawQuad(const glm::mat4& transformation, const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		if (s_data.quad_index_count >= s_data.k_max_indices)
			FlushAndReset();

		const float texture_slot{ 0.0f };

		glm::mat4 transform = transformation;

		for (int i = 0; i < 4; ++i)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[i];
			s_data.quad_vertex_buffer_ptr->color = color;
			s_data.quad_vertex_buffer_ptr->tex_coord = { i > 0 && i < 3 ? 1 : 0, i < 1 ? 1 : 0 };
			s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
			s_data.quad_vertex_buffer_ptr->entity_id = 0;
			s_data.quad_vertex_buffer_ptr++;
		}

		s_data.quad_index_count += 6;

		s_data.stats.quads++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id)
	{
		PROFILER_FUNCTION();

		if (s_data.quad_index_count >= s_data.k_max_indices)
			FlushAndReset();

		const float texture_slot{ 0.0f };

		glm::mat4 transform = transformation;

		for (int i = 0; i < 4; ++i)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertices[i];
			s_data.quad_vertex_buffer_ptr->color = color;
			s_data.quad_vertex_buffer_ptr->tex_coord = { i > 0 && i < 3 ? 1 : 0, i < 1 ? 1 : 0 };
			s_data.quad_vertex_buffer_ptr->texture_slot = texture_slot;
			s_data.quad_vertex_buffer_ptr->entity_id = entity_id;
			s_data.quad_vertex_buffer_ptr++;
		}

		s_data.quad_index_count += 6;

		s_data.stats.quads++;
	}

	void
	Renderer2D::DrawSprite(const glm::mat4& transformation, const SpriteRendererComponent& component, int32_t entity_id)
	{
		DrawQuad(transformation, component.color, entity_id);
	}

	void
	Renderer2D::DrawCircle(const glm::mat4& transformation, const CircleRendererComponent& component, int32_t entity_id)
	{
		PROFILER_FUNCTION();

		for (int i = 0; i < 4; ++i)
		{
			s_data.circle_vertex_buffer_ptr->world_position = transformation * s_data.quad_vertices[i];
			s_data.circle_vertex_buffer_ptr->local_position = s_data.quad_vertices[i] * 2.0f;
			s_data.circle_vertex_buffer_ptr->color = component.color;
			s_data.circle_vertex_buffer_ptr->thickness = component.thickness;
			s_data.circle_vertex_buffer_ptr->fade = component.fade;
			s_data.circle_vertex_buffer_ptr->entity_id = entity_id;

			s_data.circle_vertex_buffer_ptr++;
		}

		s_data.circle_index_count += 6;

		s_data.stats.quads++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int32_t entity_id)
	{
		PROFILER_FUNCTION();

		s_data.line_vertex_buffer_ptr->position = p0;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr->entity_id = entity_id;
		s_data.line_vertex_buffer_ptr++;

		s_data.line_vertex_buffer_ptr->position = p1;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr->entity_id = entity_id;
		s_data.line_vertex_buffer_ptr++;

		s_data.line_index_count += 2;
	}
	void Renderer2D::SetLineWidth(float width)
	{
		RendererCommand::SetLineWidth(width);
	}

	void
	Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, int32_t entity_id)
	{
		glm::vec3 p0 = glm::vec3(position.x - scale.x * 0.5f, position.y - scale.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + scale.x * 0.5f, position.y - scale.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - scale.x * 0.5f, position.y + scale.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entity_id);
		DrawLine(p1, p2, color, entity_id);
		DrawLine(p2, p3, color, entity_id);
		DrawLine(p3, p0, color, entity_id);

	}
	void Renderer2D::DrawRect(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id)
	{
		glm::vec3 p0 = transformation * s_data.quad_vertices[0];
		glm::vec3 p1 = transformation * s_data.quad_vertices[1];
		glm::vec3 p2 = transformation * s_data.quad_vertices[2];
		glm::vec3 p3 = transformation * s_data.quad_vertices[3];

		DrawLine(p0, p1, color, entity_id);
		DrawLine(p1, p2, color, entity_id);
		DrawLine(p2, p3, color, entity_id);
		DrawLine(p3, p0, color, entity_id);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_data.stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data.stats, 0, sizeof(Statistics));
	}

}