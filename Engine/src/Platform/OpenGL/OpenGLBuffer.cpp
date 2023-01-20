#include "OpenGLBuffer.h"

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size)
	{
		PROFILER_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);

		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLVertexBuffer::Bind()
	{
		PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLVertexBuffer::UnBind()
	{
		PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::set_layout(BufferLayout layout)
	{
		m_layout = layout;
	}
	BufferLayout& OpenGLVertexBuffer::get_layout()
	{
		return m_layout;
	}

///////////////////////////////////////////////////
// indexBuff//
//////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t count)
			:m_count{ count }
	{
		PROFILER_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PROFILER_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLIndexBuffer::Bind()
	{
		PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLIndexBuffer::UnBind()
	{
		PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace Engine