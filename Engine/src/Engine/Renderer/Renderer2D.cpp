#include "config.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer2D.h"
#include "RendererCommand.h"
#include "Batch.h"
#include "Shader.h"

namespace Engine
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		uint32_t texture_slot;

		//for editor
		int32_t entity_id{ -1 };
	};
	struct CircleVertex
	{
		glm::vec3 position;
		glm::vec3 local_position;
		glm::vec4 color;
		float thickness;
		float fade;

		//for editor
		int32_t entity_id{ -1 };
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		int32_t entity_id{ -1 };
	};

	struct Renderer2DData
	{
		Renderer2DData()
		{
			quad_batch = CreateScope<Batch<QuadVertex>>(
					BufferLayout({
							{ ShaderDataType::Float3, "a_position" },
							{ ShaderDataType::Float4, "a_color" },
							{ ShaderDataType::Float2, "a_tex_coord" },
							{ ShaderDataType::Int, "a_tex_slot" },
							{ ShaderDataType::Int, "a_entity_id" }})
			);
			quad_shader = Shader::Create(CMAKE_SOURCE_DIR"/Engine/assets/shaders/2D_quad_shader.glsl");

			int sampler[Renderer2D::k_max_texture_slot];
			for (int i = 0; i < Renderer2D::k_max_texture_slot; ++i)
			{
				sampler[i] = i;
			}
			quad_shader->Bind();
			quad_shader->SetIntArray("u_texture2D", sampler, Renderer2D::k_max_texture_slot);

			circle_batch = CreateScope<Batch<CircleVertex>>(
					BufferLayout({
							{ ShaderDataType::Float3, "a_position", },
							{ ShaderDataType::Float3, "a_local" },
							{ ShaderDataType::Float4, "a_color" },
							{ ShaderDataType::Float, "a_thickness" },
							{ ShaderDataType::Float, "a_fade", },
							{ ShaderDataType::Int, "a_entity_id" }
					}));
			circle_shader = Shader::Create(CMAKE_SOURCE_DIR"/Engine/assets/shaders/2D_circle_shader.glsl");

			line_batch = CreateScope<Batch<LineVertex>>(
					BufferLayout({
							{ ShaderDataType::Float3, "a_position", },
							{ ShaderDataType::Float4, "a_color" },
							{ ShaderDataType::Int, "a_entity_id" }
					}));
			line_shader = Shader::Create(CMAKE_SOURCE_DIR"/Engine/assets/shaders/2D_line_shader.glsl");

			quad_vertices[0] = { -0.5f, -0.5f, 0, 1 };
			quad_vertices[1] = { 0.5f, -0.5f, 0, 1 };
			quad_vertices[2] = { 0.5f, 0.5f, 0, 1 };
			quad_vertices[3] = { -0.5f, 0.5f, 0, 1 };

			white_texture = Texture2D::Create(1, 1);
			uint32_t tex_data = 0xffffffff;
			white_texture->SetData(&tex_data, sizeof(tex_data));
			textures[0] = white_texture;

			camera_matrices = UniformBuffer::Create(
					{{ ShaderDataType::Mat4, "projection" }, { ShaderDataType::Mat4, "view" }}, 2);
		}

		Scope<Batch<QuadVertex>> quad_batch;
		Scope<Batch<CircleVertex>> circle_batch;
		Scope<Batch<LineVertex>> line_batch;

		std::array<glm::vec4, 4> quad_vertices;

		Ref<Shader> quad_shader;
		Ref<Shader> circle_shader;
		Ref<Shader> line_shader;

		Ref<Texture> white_texture;
		std::array<Ref<Texture>, Renderer2D::k_max_texture_slot> textures;
		uint32_t texture_count = { 1 };

		Ref<UniformBuffer> camera_matrices;

		Renderer2D::Statistics stats;
	};

	Renderer2DData* Renderer2D::s_data = nullptr;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DData();
	}
	void Renderer2D::ShutDown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const glm::mat4& projection, const glm::mat4& view)
	{
		s_data->camera_matrices->Bind();
		s_data->camera_matrices->Set({ ShaderDataType::Mat4, "projection" },
				(const void*)glm::value_ptr(projection));
		s_data->camera_matrices->Set({ ShaderDataType::Mat4, "view" },
				(const void*)glm::value_ptr(view));

		s_data->quad_batch->Begin();
		s_data->circle_batch->Begin();
		s_data->line_batch->Begin();
	}

	void Renderer2D::EndScene()
	{
		s_data->quad_batch->End();
		s_data->circle_batch->End();
		s_data->line_batch->End();

		FlushQuad();
		FlushCircle();
		FlushLine();
	}

	void Renderer2D::FlushQuad()
	{
		uint32_t index_count = s_data->quad_batch->GetVertexCount() * 1.5f;
		if (index_count)
		{
			Ref<VertexArray> va = s_data->quad_batch->Flush();

			for (int i = 0; i < s_data->texture_count; ++i)
			{
				s_data->textures[i]->Bind(i);
			}
			s_data->quad_shader->Bind();
			RendererCommand::DrawIndex(va, index_count);

			s_data->stats.draw_calls++;
		}
	}

	void Renderer2D::FlushCircle()
	{
		uint32_t index_count = s_data->circle_batch->GetVertexCount() * 1.5f;
		if (index_count)
		{
			Ref<VertexArray> va = s_data->circle_batch->Flush();
			s_data->circle_shader->Bind();
			RendererCommand::DrawIndex(va, index_count);

			s_data->stats.draw_calls++;
		}
	}

	void Renderer2D::FlushLine()
	{
		uint32_t index_count = s_data->line_batch->GetVertexCount();
		if (index_count)
		{
			Ref<VertexArray> va = s_data->line_batch->Flush();

			s_data->line_shader->Bind();
			RendererCommand::DrawLine(va, index_count);

			s_data->stats.draw_calls++;
		}
	}

	void Renderer2D::DrawQuad(const glm::mat4& transformation,
			const Ref<Texture>& texture, const glm::vec4& color, int32_t entity_id)
	{
		uint32_t texture_slot{ 0 };
		if (texture)
		{
			for (int i = 1; i < s_data->texture_count; ++i)
			{
				if (texture->GetRendererId() == s_data->textures[i]->GetRendererId())
				{
					texture_slot = i;
				}
			}
			if (texture_slot == 0)
			{
				texture_slot = s_data->texture_count;
				s_data->textures[texture_slot] = texture;
				s_data->texture_count++;
			}
		}

		bool full = false;
		for (int i = 0; i < 4; ++i)
		{
			full |= !s_data->quad_batch->AddIfCan(
					{ transformation * s_data->quad_vertices[i],
					  color,
					  { i > 0 && i < 3 ? 1 : 0, i > 1 ? 1 : 0 },
					  texture_slot,
					  entity_id });
		}

		if (full)
		{
			s_data->quad_batch->End();
			FlushQuad();
			WARN(" Batch overflow");
		};

		s_data->stats.quads++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transformation, const Ref<SubTexture2D>& sub_texture,
			const glm::vec4& color, int32_t entity_id)
	{
		uint32_t texture_slot{ 0 };
		if (sub_texture)
		{
			for (int i = 1; i < s_data->texture_count; ++i)
			{
				if (sub_texture->GetTexture()->GetRendererId() == s_data->textures[i]->GetRendererId())
				{
					texture_slot = i;
				}
			}

			if (texture_slot == 0 && s_data->texture_count < k_max_texture_slot)
			{
				texture_slot = s_data->texture_count;
				s_data->textures[texture_slot] = sub_texture->GetTexture();
				s_data->texture_count++;
			}

			if (s_data->texture_count == k_max_texture_slot)
			{
				texture_slot = k_max_texture_slot - 1;
				s_data->quad_batch->End();
				FlushQuad();
				CORE_WARN(" Batch overflow. Too many Textures");
			}
		}

		bool full = false;
		for (int i = 0; i < 4; ++i)
		{
			full |= !s_data->quad_batch->AddIfCan(
					{ transformation * s_data->quad_vertices[i],
					  color,
					  sub_texture->GetCoords()[i],
					  texture_slot,
					  entity_id });
		}

		if (full)
		{
			s_data->quad_batch->End();
			FlushQuad();
			CORE_WARN(" Batch overflow. Too many vertices");
		};

		s_data->stats.quads++;
	}

	void
	Renderer2D::DrawCircle(const glm::mat4& transformation, const glm::vec4& color, float thickness, float fade,
			int32_t entity_id)
	{
		bool full = false;
		for (int i = 0; i < 4; ++i)
		{
			full |= !s_data->circle_batch->AddIfCan(
					{ transformation * s_data->quad_vertices[i],
					  s_data->quad_vertices[i] * 2.0f,
					  color,
					  thickness,
					  fade,
					  entity_id });
		}

		if (full)
		{
			s_data->circle_batch->End();
			FlushCircle();
			WARN(" Batch overflow");
		};

		s_data->stats.circles++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness,
			int32_t entity_id)
	{
		SetLineWidth(thickness);

		bool full = false;
		LineVertex l0 = { p0, color, entity_id };
		LineVertex l1 = { p1, color, entity_id };

		full |= !s_data->line_batch->AddIfCan(l0);
		full |= !s_data->line_batch->AddIfCan(l1);

		if (full)
		{
			s_data->line_batch->End();
			FlushLine();
			WARN(" Batch overflow");
		};

		s_data->stats.lines++;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		RendererCommand::SetLineWidth(width);
	}

	void
	Renderer2D::DrawRect(const glm::mat4& transformation, const glm::vec4& color, float thickness, int32_t entity_id)
	{
		glm::vec3 p0 = { transformation * s_data->quad_vertices[0] };
		glm::vec3 p1 = { transformation * s_data->quad_vertices[1] };
		glm::vec3 p2 = { transformation * s_data->quad_vertices[2] };
		glm::vec3 p3 = { transformation * s_data->quad_vertices[3] };

		DrawLine(p0, p1, color, thickness, entity_id);
		DrawLine(p1, p2, color, thickness, entity_id);
		DrawLine(p2, p3, color, thickness, entity_id);
		DrawLine(p3, p0, color, thickness, entity_id);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_data->stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data->stats, 0, sizeof(Renderer2D::Statistics));
	}

}

