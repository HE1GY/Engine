#include "OpenGLBuffer.h"

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		PROFILER_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

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

	void OpenGLVertexBuffer::SetData(void* data, int32_t size)
	{
		PROFILER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void OpenGLVertexBuffer::SetLayout(BufferLayout layout)
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


	////////uniform buffer

	OpenGLUniformBuffer::OpenGLUniformBuffer(BufferLayout layout, uint32_t binding_point)
			:m_layout{ layout }, m_binding_point{ binding_point }
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
		glBufferData(GL_UNIFORM_BUFFER, layout.get_stride(), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, m_renderer_id);
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}
	void OpenGLUniformBuffer::Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
	}
	void OpenGLUniformBuffer::UnBind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void OpenGLUniformBuffer::Set(BufferElement element, const void* value)
	{
		uint32_t offset{ 0 };
		for (const auto& buffer_element : m_layout)
		{
			if (buffer_element == element)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id);
				uint32_t size = ShaderDataTypeSize(buffer_element.type);
				glBufferSubData(GL_UNIFORM_BUFFER, offset, size,
						value);
				break;
			}

			uint32_t base_alignment = ShaderDataTypeSize(element.type);
			CORE_ASSERT((base_alignment == 64), "Uniform buffer support only mat4");
			if (offset % base_alignment == 0)
			{
				offset += base_alignment;
			}
			else
			{//multiple of base_alignment > offset
				for (int i = 0; i < 1000; ++i) // TODO :: new algo
				{
					if (base_alignment * i > offset)
					{
						offset = base_alignment * i; //search aligned offset
						break;
					}
				}
			}
		}
	}
}