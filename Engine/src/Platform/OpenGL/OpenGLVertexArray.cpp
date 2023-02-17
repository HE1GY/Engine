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

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		PROFILER_FUNCTION();

		glBindVertexArray(m_renderer_id);
		indexBuffer->Bind();
		m_indexBuffer = indexBuffer;
	}
	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		PROFILER_FUNCTION();

		CORE_ASSERT(vertexBuffer->get_layout().get_elements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_renderer_id);

		vertexBuffer->Bind();

		BufferLayout& layout = vertexBuffer->get_layout();
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
						(GLenum*)element.offset);
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

		m_vertexBuffers.push_back(vertexBuffer);
	}
	std::shared_ptr<IndexBuffer> OpenGLVertexArray::get_indexBuffer()
	{
		return m_indexBuffer;
	}

} // namespace Engine
