#pragma once

#include "OrthographicCamera.h"
#include "RendererCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(SceneCamera& camera);
		static void EndScene();

		static void OnWindowResized(uint32_t width, uint32_t height);

		static void Submit(const std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray> vertex_array,
				const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API get_renderer_API()
		{
			return RendererAPI::get_renderer_API();
		}

	private:
		struct SceneData
		{
			glm::mat4 view_projection_matrix;
		};

		static SceneData* s_scene_data;
	};

} // namespace Engine