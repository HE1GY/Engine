#include <glm/ext/matrix_transform.hpp>

#include "Renderer2D.h"
#include "RendererCommand.h"
#include "Batch2D.h"

namespace Engine
{
	struct Renderer2DData
	{
		Renderer2DData()
		{
			quad_batch = CreateScope<Batch2D<QuadVertex>>(
					"../../../Sandbox/assets/shaders/default_2D_shader.glsl", Primitive::Quad);

			circle_batch = CreateScope<Batch2D<CircleVertex>>(
					"../../../Sandbox/assets/shaders/default_2D_circle_shader.glsl", Primitive::Circle);

			line_batch = CreateScope<Batch2D<LineVertex>>(
					"../../../Sandbox/assets/shaders/default_2D_line_shader.glsl", Primitive::Line);

			white_texture = Texture2D::Create(1, 1);
			uint32_t tex_data = 0xffffffff;
			white_texture->SetData(&tex_data, sizeof(tex_data));
		}

		Scope<Batch2D<QuadVertex>> quad_batch;
		Scope<Batch2D<CircleVertex>> circle_batch;
		Scope<Batch2D<LineVertex>> line_batch;

		Ref<Texture> white_texture;

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
		//Uniform buffer

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
		uint32_t index_count = s_data->quad_batch->GetIndexCount();
		Ref<VertexArray> va = s_data->quad_batch->Flush();

		RendererCommand::DrawIndex(va, index_count);

		s_data->stats.draw_calls++;
	}
	void Renderer2D::FlushCircle()
	{
		uint32_t index_count = s_data->circle_batch->GetIndexCount();
		std::shared_ptr<VertexArray> va = s_data->circle_batch->Flush();

		RendererCommand::DrawIndex(va, index_count);

		s_data->stats.draw_calls++;
	}
	void Renderer2D::FlushLine()
	{
		uint32_t index_count = s_data->line_batch->GetIndexCount();
		std::shared_ptr<VertexArray> va = s_data->line_batch->Flush();

		RendererCommand::DrawLine(va, index_count);

		s_data->stats.draw_calls++;
	}

	//
	void Renderer2D::DrawQuad(const glm::mat4& transformation,
			const Ref<Texture>& texture, const glm::vec4& color, int32_t entity_id)
	{
		bool full = !s_data->quad_batch->Add(transformation, texture, color, entity_id);

		if (full)
		{
			FlushQuad();
		}

		s_data->stats.quads++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id)
	{
		bool full = !s_data->quad_batch->Add(transformation, s_data->white_texture, color, entity_id);

		if (full)
		{
			FlushQuad();
		}

		s_data->stats.quads++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transformation, const Ref<SubTexture2D>& texture,
			const glm::vec4& color, int32_t entity_id)
	{
		//TODO sub texture

		bool full = !s_data->quad_batch->Add(transformation, s_data->white_texture, color, entity_id);

		if (full)
		{
			FlushQuad();
		}
		s_data->stats.quads++;
	}

	//
	void
	Renderer2D::DrawCircle(const glm::mat4& transformation, const glm::vec4& color, float thickness, float fade,
			int32_t entity_id)
	{
		bool full = !s_data->circle_batch->Add(transformation, color, thickness, fade, entity_id);

		if (full)
		{
			FlushCircle();
		}
		s_data->stats.circles++;
	}
	//
	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness,
			int32_t entity_id)
	{
		SetLineWidth(thickness);
		bool full = !s_data->line_batch->Add(p0, p1, color, entity_id);

		if (full)
		{
			FlushLine();
		}

		s_data->stats.lines++;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		RendererCommand::SetLineWidth(width);
	}

	//
	void Renderer2D::DrawRect(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id)
	{
		//TODO
	}
	//

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_data->stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data->stats, 0, sizeof(Renderer2D::Statistics));
	}

}

