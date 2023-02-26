#pragma once

#include "Shader.h"
#include "Camera.h"
#include "VertexArray.h"
#include "RendererAPI.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const glm::mat4& projection, const glm::mat4& view);
		static void EndScene();

		static void OnWindowResized(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetRendererApi()
		{
			return RendererAPI::GetRendererApi();
		}
	};

} // namespace Engine