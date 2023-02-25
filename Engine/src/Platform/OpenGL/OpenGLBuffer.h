#pragma once

#include "glad/glad.h"

#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(void* data, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual void SetLayout(BufferLayout layout) override;
		virtual BufferLayout& GetLayout() override;

	private:
		GLuint m_renderer_id;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* data, uint32_t count);
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

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(BufferLayout layout, uint32_t binding_point);
		virtual ~OpenGLUniformBuffer();
		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Set(BufferElement element, const void* value) override;
	private:
		BufferLayout m_layout;
		uint32_t m_renderer_id;
		uint32_t m_binding_point;
	};
} // namespace Engine