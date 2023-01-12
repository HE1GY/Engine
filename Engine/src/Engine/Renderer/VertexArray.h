#pragma once
#include "Buffer.h"

namespace Engine
{
	class VertexArray
	{
	public:
		static VertexArray* Create();

		virtual ~VertexArray()
		{
		};

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffers) = 0;
		virtual void set_indexBuffer(std::shared_ptr<IndexBuffer> index_buffer) = 0;
		virtual std::shared_ptr<IndexBuffer> get_indexBuffer() = 0;
	};
}