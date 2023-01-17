#pragma once
#include "RendererAPI.h"

namespace Engine
{
	class RendererCommand
	{
	public:
		inline static void Init()
		{
			m_renderer_API->Init();
		}
		inline static void DrawIndex(std::shared_ptr<VertexArray> vertex_array)
		{
			m_renderer_API->DrawIndex(vertex_array);
		}
		inline static void Clear()
		{
			m_renderer_API->Clear();
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			m_renderer_API->SetClearColor(color);
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			m_renderer_API->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* m_renderer_API;
	};
}