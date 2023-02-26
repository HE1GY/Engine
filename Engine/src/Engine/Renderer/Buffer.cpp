#include "pch.h"

#include "Buffer.h"
#include "Engine/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Engine
{
	uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 2 * 4;
		case ShaderDataType::Float3:
			return 3 * 4;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 3 * 3 * 4;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		default:
		CORE_ASSERT(false, "Unsupported shader type");
			return 0;
		}
	}

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
			:name{ name }, type{ type }, normalized{ normalized }, offset{ 0 }, size{ 0 }
	{
		size = ShaderDataTypeSize(type);
	}

	uint32_t BufferElement::GetComponentCount() const
	{

		switch (type)
		{
		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return 2;
		case ShaderDataType::Float3:
			return 3;
		case ShaderDataType::Float4:
			return 4;
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Mat3:
			return 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		default:
		CORE_ASSERT(false, "Unsupported shader type");
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

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetRendererApi())
		{
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size);

		default:
		CORE_ASSERT(false, "Renderer API = None");
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::GetRendererApi())
		{
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(data, size);

		default:
		CORE_ASSERT(false, "Renderer API = None");
			return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count)
	{
		switch (RendererAPI::GetRendererApi())
		{
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(data, count);

		default:
		CORE_ASSERT(false, "Renderer API = None");
			return nullptr;
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(BufferLayout layout, uint32_t binding_point)
	{
		switch (RendererAPI::GetRendererApi())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(layout, binding_point);

		default:
		CORE_ASSERT(false, "Renderer API = None");
			return nullptr;
		}
	}

} // namespace Engine