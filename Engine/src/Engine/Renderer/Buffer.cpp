#include "pch.h"

#include "Buffer.h"
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Engine
{
VertexBuffer *VertexBuffer::Create(void *data, uint32_t size)
{
    switch (Renderer::get_renderer_API())
    {
    case Renderer::API::None:
        return nullptr;
        ASSERT(0);
        CORE_ERROR("API = None");

    case Renderer::API::OpenGL:
        return new OpenGLVertexBuffer(data, size);

    default:
        return nullptr;
        ASSERT(0);
        CORE_ERROR("API = None");
    }
}

IndexBuffer *IndexBuffer::Create(uint32_t *data, uint32_t count)
{
    switch (Renderer::get_renderer_API())
    {
    case Renderer::API::None:
        return nullptr;
        ASSERT(0);
        CORE_ERROR("API = None");

    case Renderer::API::OpenGL:
        return new OpenGLIndexBuffer(data, count);

    default:
        return nullptr;
        ASSERT(0);
        CORE_ERROR("API = None");
    }
}
} // namespace Engine