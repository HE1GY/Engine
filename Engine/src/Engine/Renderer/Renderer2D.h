#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Texture.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "VertexArray.h"

namespace Engine
{
	struct Renderer2DData;

	class Renderer2D
	{
	public :
		static void Init();
		static void ShutDown();
		static void Flush();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		static void
		DrawQuad(const glm::vec3& position, const glm::vec2& scale, const float rotation, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture>& texture,
				const glm::vec4& color = { 1, 1, 1, 1 });
	private:
		static Renderer2DData s_data;
	};
}
