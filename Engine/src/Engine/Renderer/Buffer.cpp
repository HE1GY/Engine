#include "pch.h"

#include "Engine/Core.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Engine
{
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;
			break;
		case ShaderDataType::Float:
			return 4;
			break;
		case ShaderDataType::Float2:
			return 2 * 4;
			break;
		case ShaderDataType::Float3:
			return 3 * 4;
			break;
		case ShaderDataType::Float4:
			return 4 * 4;
			break;
		case ShaderDataType::Int:
			return 4;
			break;
		case ShaderDataType::Int2:
			return 4 * 2;
			break;
		case ShaderDataType::Int3:
			return 4 * 3;
			break;
		case ShaderDataType::Int4:
			return 4 * 4;
			break;
		case ShaderDataType::Mat3:
			return 3 * 3 * 4;
			break;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
			break;
		case ShaderDataType::Bool:
			return 1;
			break;
		default:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;
		}
	}

	BufferElement::BufferElement(const std::string& name, ShaderDataType type, bool normalized)
			:name{ name }, type{ type }, normalized{ normalized }, offset{ 0 }, size{ 0 }
	{
		size = ShaderDataTypeSize(type);
	}

	uint32_t BufferElement::GetComponentCount() const
	{

		switch (type)
		{

		case ShaderDataType::None:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;
			break;
		case ShaderDataType::Float:
			return 1;
			break;
		case ShaderDataType::Float2:
			return 2;
			break;
		case ShaderDataType::Float3:
			return 3;
			break;
		case ShaderDataType::Float4:
			return 4;
			break;
		case ShaderDataType::Int:
			return 1;
			break;
		case ShaderDataType::Int2:
			return 2;
			break;
		case ShaderDataType::Int3:
			return 3;
			break;
		case ShaderDataType::Int4:
			return 4;
			break;
		case ShaderDataType::Mat3:
			return 3 * 3;
			break;
		case ShaderDataType::Mat4:
			return 4 * 4;
			break;
		case ShaderDataType::Bool:
			return 1;
			break;
		default:
		CORE_ASSERT(false, "BufferLayout None type");
			return 0;

		}
	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> layout)
			:m_elements{ layout }
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		m_stride = 0;
		uint32_t offset = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}

	VertexBuffer* VertexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::get_renderer_API())
		{
		case RendererAPI::API::None:
		CORE_ASSERT(false, "API = None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(data, size);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* data, uint32_t count)
	{
		switch (RendererAPI::get_renderer_API())
		{
		case RendererAPI::API::None:
		CORE_ASSERT(false, "API = None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(data, count);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}
	}
} // namespace Engine