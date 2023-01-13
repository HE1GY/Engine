#include "pch.h"

#include "glad/glad.h"

#include "OpenGLVertexArray.h"
#include "Engine/Core.h"

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
		glCreateVertexArrays(1, &m_renderer_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_renderer_id);
	}
	void OpenGLVertexArray::UnBind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::set_indexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		glBindVertexArray(m_renderer_id);
		indexBuffer->Bind();
		m_indexBuffer = indexBuffer;
	}
	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		CORE_ASSERT(vertexBuffer->get_layout().get_elements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_renderer_id);

		vertexBuffer->Bind();

		BufferLayout& layout = vertexBuffer->get_layout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexBuffer_index);
			glVertexAttribPointer(m_vertexBuffer_index, element.GetComponentCount(),
					ShaderDataTypeToOpenGL(element.type),
					element.normalized ? GL_TRUE : GL_FALSE, layout.get_stride(), (const void*)element.offset);
			m_vertexBuffer_index++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}
	std::shared_ptr<IndexBuffer> OpenGLVertexArray::get_indexBuffer()
	{
		return m_indexBuffer;
	}

}