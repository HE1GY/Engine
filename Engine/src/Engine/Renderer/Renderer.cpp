#include "pch.h"
#include "Renderer.h"

namespace Engine
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(std::shared_ptr<VertexArray> vertex_array)
	{
		vertex_array->Bind();
		RendererCommand::DrawIndex(vertex_array);
	}

}