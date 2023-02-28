#include "pch.h"

#include "glad/glad.h"

#include "Engine/Core/Core.h"
#include "OpenGLVertexArray.h"

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;
		case ShaderDataType::Float:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float2:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float3:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float4:
			return GL_FLOAT;
			break;
		case ShaderDataType::Int:
			return GL_INT;
			break;
		case ShaderDataType::Int2:
			return GL_INT;
			break;
		case ShaderDataType::Int3:
			return GL_INT;
			break;
		case ShaderDataType::Int4:
			return GL_INT;
			break;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
			break;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
			break;
		case ShaderDataType::Bool:
			return GL_BOOL;
			break;
		default:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		PROFILER_FUNCTION();

		glCreateVertexArrays(1, &m_renderer_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PROFILER_FUNCTION();

		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void OpenGLVertexArray::Bind()
	{
		PROFILER_FUNCTION();

		glBindVertexArray(m_renderer_id);
	}
	void OpenGLVertexArray::UnBind()
	{
		PROFILER_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> index_buffer)
	{
		PROFILER_FUNCTION();

		glBindVertexArray(m_renderer_id);
		index_buffer->Bind();
		m_index_buffer = index_buffer;
	}
	void OpenGLVertexArray::SetVertexBuffer(Ref<VertexBuffer> vertex_buffer)
	{
		PROFILER_FUNCTION();

		CORE_ASSERT(vertex_buffer->GetLayout().get_elements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_renderer_id);

		vertex_buffer->Bind();

		BufferLayout& layout = vertex_buffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::None:
				break;
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				glEnableVertexAttribArray(m_vertexBuffer_index);
				glVertexAttribPointer(m_vertexBuffer_index, element.GetComponentCount(),
						ShaderDataTypeToOpenGL(element.type),
						element.normalized ? GL_TRUE : GL_FALSE, layout.get_stride(),
						(GLenum*)static_cast<int32_t>(element.offset));
				m_vertexBuffer_index++;
				break;

			case ShaderDataType::Bool:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				glEnableVertexAttribArray(m_vertexBuffer_index);
				glVertexAttribIPointer(m_vertexBuffer_index, element.GetComponentCount(),
						ShaderDataTypeToOpenGL(element.type),
						layout.get_stride(),
						(GLenum*)element.offset);
				m_vertexBuffer_index++;
				break;
			}

		}

		m_vertex_buffer = vertex_buffer;
	}
	Ref<IndexBuffer> OpenGLVertexArray::GetIndexBuffer()
	{
		return m_index_buffer;
	}

} // namespace Engine
