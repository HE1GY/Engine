#pragma once
#include "RendererCommand.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void BeginScene();

		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray> vertex_array);

		inline static RendererAPI::API get_renderer_API()
		{
			return RendererAPI::get_renderer_API();
		}

	};

} // namespace Engine