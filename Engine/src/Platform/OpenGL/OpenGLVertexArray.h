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

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;
		virtual std::shared_ptr<IndexBuffer> get_indexBuffer() override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		uint32_t m_renderer_id;
		uint32_t m_vertexBuffer_index{ 0 };
	};

}
