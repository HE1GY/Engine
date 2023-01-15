#include "pch.h"

#include "Texture.h"
#include "RendererAPI.h"
#include <Platform/OpenGL/OpenGLTexture2D.h>

namespace Engine
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::get_renderer_API())
		{
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}
	}
}