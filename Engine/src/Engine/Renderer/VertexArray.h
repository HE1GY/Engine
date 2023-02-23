#pragma once
#include "Buffer.h"
#include <Engine/Core/Core.h>

namespace Engine
{
	class VertexArray
	{
	public:
		static Ref<VertexArray> Create();

		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetVertexBuffer(Ref<VertexBuffer> vertex_buffers) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> index_buffer) = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() = 0;
	};
} // namespace Engine