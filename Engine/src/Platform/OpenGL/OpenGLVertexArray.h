#pragma once
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;
		virtual Ref<IndexBuffer> GetIndexBuffer() override;

	private:
		Ref<VertexBuffer> m_vertex_buffer;
		Ref<IndexBuffer> m_index_buffer;
		uint32_t m_renderer_id;
		uint32_t m_vertexBuffer_index{ 0 };
	};

}
