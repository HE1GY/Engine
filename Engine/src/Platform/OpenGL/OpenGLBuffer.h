#pragma once

#include "glad/glad.h"

#include "Engine/Renderer/Buffer.h"

namespace Engine
{
class OpenGLVertexBuffer : public VertexBuffer
{
  public:
    OpenGLVertexBuffer(void *data, uint32_t size);
    virtual ~OpenGLVertexBuffer();
    virtual void Bind() override;
    virtual void UnBind() override;

  private:
    GLuint m_renderer_id;
};

class OpenGLIndexBuffer : public IndexBuffer
{
  public:
    OpenGLIndexBuffer(uint32_t *data, uint32_t count);
    virtual ~OpenGLIndexBuffer();
    virtual void Bind() override;
    virtual void UnBind() override;

    inline uint32_t get_count() override
    {
        return m_count;
    };

  private:
    GLuint m_renderer_id;
    uint32_t m_count;
};
} // namespace Engine