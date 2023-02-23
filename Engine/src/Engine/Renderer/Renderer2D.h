#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "SubTexture2D.h"

namespace Engine
{
	struct Renderer2DData;

	class Renderer2D
	{
	public :
		struct Statistics
		{
			uint32_t draw_calls{ 0 };
			uint32_t quads{ 0 };
			uint32_t circles{ 0 };
			uint32_t lines{ 0 };
			uint32_t GetIndices()
			{
				return quads * 6 + circles * 6 + lines * 2;
			}
			uint32_t GetVertices()
			{
				return quads * 4 + circles * 4 + lines * 2;
			}
		};

		static const uint32_t k_max_quads = 10000;
		static const uint32_t k_max_vertices = k_max_quads * 4;
		static const uint32_t k_max_indices = k_max_quads * 6;
		static const uint32_t k_max_texture_slot = 32;

		static void Init();
		static void ShutDown();

		static void BeginScene(const glm::mat4& projection, const glm::mat4& view);

		static void EndScene();
		static void FlushQuad();
		static void FlushCircle();
		static void FlushLine();

		static void DrawQuad(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id);

		static void DrawQuad(const glm::mat4& transformation, const Ref<SubTexture2D>& texture,
				const glm::vec4& color, int32_t entity_id);

		static void DrawQuad(const glm::mat4& transformation,
				const Ref<Texture>& texture, const glm::vec4& color, int32_t entity_id);

		static void DrawCircle(const glm::mat4& transformation, const glm::vec4& color, float thickness, float fade,
				int32_t entity_id);

		static void
		DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness, int32_t entity_id);

		static void DrawRect(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id);

		static void SetLineWidth(float width);

		static Statistics GetStats();
		static void ResetStats();
	private:
		static Renderer2DData* s_data;
	};
}
