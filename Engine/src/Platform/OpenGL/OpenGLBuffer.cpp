#include "OpenGLBuffer.h"

namespace Engine
{
OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32_t size)
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void OpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void OpenGLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

///////////////////////////////////////////////////
// indexBuff//
//////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *data, uint32_t count) : m_count{count}
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void OpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void OpenGLIndexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace Engine