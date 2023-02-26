#include "FrameBuffer.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Engine
{

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (RendererAPI::GetRendererApi())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(spec);

		default:
		CORE_ASSERT(false, "Renderer API = None");
			return nullptr;
		}
	}
}