#include "pch.h"

#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI* RendererCommand::m_renderer_API = new OpenGLRendererAPI();
}