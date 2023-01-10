#pragma once

#include <stdint.h>

namespace Engine
{
class VertexBuffer
{

  public:
    virtual ~VertexBuffer(){};

    static VertexBuffer *Create(void *data, uint32_t size);

    virtual void Bind() = 0;
    virtual void UnBind() = 0;
};

class IndexBuffer
{
  public:
    virtual ~IndexBuffer(){};

    static IndexBuffer *Create(uint32_t *data, uint32_t count);

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual inline uint32_t get_count() = 0;
};
}; // namespace Engine