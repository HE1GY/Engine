#include "pch.h"

#include "Engine/Core.h"
#include "Shader.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Shader* Shader::Create(std::string vertex_src, std::string fragment_src)
	{
		switch (RendererAPI::get_renderer_API())
		{

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertex_src, fragment_src);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}

	}
} // namespace Engine