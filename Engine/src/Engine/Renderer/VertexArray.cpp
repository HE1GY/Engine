#include "pch.h"

#include "Engine/Core/Core.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

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
} // namespace Engine