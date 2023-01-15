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

	private:
		static RendererAPI* m_renderer_API;
	};
}