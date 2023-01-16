#include "pch.h"

#include "Renderer.h"
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::get_renderer_API())
		{
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}
	}
}