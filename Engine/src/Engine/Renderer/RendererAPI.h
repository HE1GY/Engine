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

		virtual void Init() = 0;
		virtual void DrawIndex(std::shared_ptr<VertexArray> vertex_array, uint32_t index_count = 0) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	private:
		static API s_render_API;
	};
}