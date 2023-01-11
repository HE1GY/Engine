#include "pch.h"
#include "Renderer.h"
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::get_renderer_API())
		{
		case Renderer::API::None:
		CORE_ASSERT(false, "API = None");
			return nullptr;

		case Renderer::API::OpenGL:
			return new OpenGLVertexArray();

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}
	}
}