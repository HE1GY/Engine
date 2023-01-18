#pragma once
#include "Buffer.h"
#include <Engine/Core/Core.h>

namespace Engine
{
class VertexArray
{
  public:
    static Ref<VertexArray> Create();

    virtual ~VertexArray() = default;

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffers) = 0;
    virtual void set_indexBuffer(std::shared_ptr<IndexBuffer> index_buffer) = 0;
    virtual std::shared_ptr<IndexBuffer> get_indexBuffer() = 0;
};
} // namespace Engine