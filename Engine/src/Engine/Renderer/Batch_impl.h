

#include <cstdint>
#include "Buffer.h"
#include "Renderer2D.h"
#include "Batch.h"

namespace Engine
{
	static uint32_t* CreateIndexBuffer()
	{
		uint32_t* line_indices = new uint32_t[Renderer2D::k_max_indices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2D::k_max_indices; i += 6)
		{
			line_indices[i + 0] = offset + 0;
			line_indices[i + 1] = offset + 1;
			line_indices[i + 2] = offset + 2;

			line_indices[i + 3] = offset + 2;
			line_indices[i + 4] = offset + 3;
			line_indices[i + 5] = offset + 0;

			offset += 4;
		}
		return line_indices;
	};
	template<typename TVertex>
	Batch<TVertex>::Batch(BufferLayout layout)
	{
		m_va = VertexArray::Create();
		m_va->Bind();

		m_vb = VertexBuffer::Create(Renderer2D::k_max_vertices * sizeof(TVertex));
		m_vb->Bind();
		m_vb->SetLayout(layout);
		m_va->SetVertexBuffer(m_vb);

		m_vertex_buffer_base = new TVertex[Renderer2D::k_max_vertices];

		uint32_t* ib = CreateIndexBuffer();

		m_ib = IndexBuffer::Create(ib, Renderer2D::k_max_vertices);
		m_ib->Bind();
		m_va->SetIndexBuffer(m_ib);
		m_va->UnBind();

		delete[] ib;
	}

	template<typename TVertex>
	Batch<TVertex>::~Batch()
	{
		delete[] m_vertex_buffer_base;
	}

	template<typename TVertex>
	Ref<VertexArray> Batch<TVertex>::Flush()
	{
		m_va->Bind();
		return m_va;
	}
	template<typename TVertex>
	bool Batch<TVertex>::AddIfCan(TVertex vertex)
	{
		*m_vertex_buffer_ptr = vertex;
		m_vertex_buffer_ptr++;

		m_vertex_count++;

		return m_vertex_count <= Renderer2D::k_max_vertices;
	}

	template<typename TVertex>
	void Batch<TVertex>::Begin()
	{
		m_vertex_buffer_ptr = m_vertex_buffer_base;
		m_vertex_count = 0;
	}

	template<typename TVertex>
	void Batch<TVertex>::End()
	{
		uint32_t size = (m_vertex_buffer_ptr - m_vertex_buffer_base) * sizeof(TVertex);
		if (size)
		{
			m_vb->SetData(m_vertex_buffer_base, size);
		}
	}

}