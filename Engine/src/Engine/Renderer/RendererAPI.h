#pragma once

#include "VertexArray.h"
#include <glm/vec4.hpp>

namespace Engine
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL,
		};

		inline static API get_renderer_API()
		{
			return s_render_API;
		}

		virtual void DrawIndex(std::shared_ptr<VertexArray> vertex_array) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

	private:
		static API s_render_API;
	};
}