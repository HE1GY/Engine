#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <Engine/Scene/Components.h>
#include "Texture.h"
#include "SubTexture2D.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "VertexArray.h"
#include "Camera.h"
#include "EditorCamera.h"

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
			uint32_t get_indices()
			{
				return quads * 6;
			}
			uint32_t get_vertices()
			{
				return quads * 4;
			}
		};

		static void Init();
		static void ShutDown();
		static void Flush();

		static void BeginScene(Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(OrthographicCamera& camera);//TODO: Remove
		static void EndScene();

		static void DrawQuad(const glm::mat4& transformation, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transformation, const glm::vec4& color, int32_t entity_id);

		static void DrawSprite(const glm::mat4& transformation, SpriteRendererComponent& component, int32_t entity_id);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		static void
		DrawQuad(const glm::vec3& position, const glm::vec2& scale, const float rotation_rad, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<SubTexture2D>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SubTexture2D>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });

		static Statistics GetStats();
		static void ResetStats();
	private:
		static void FlushAndReset();
		static Renderer2DData s_data;
	};
}
