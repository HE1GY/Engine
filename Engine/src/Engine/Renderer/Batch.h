#pragma once

#include <cstdint>
#include "VertexArray.h"
#include "Engine/Core/Core.h"

namespace Engine
{
	template<typename TVertex>
	class Batch
	{
	public:
		Batch(BufferLayout layout);
		~Batch();

		void Begin();
		void End();

		bool AddIfCan(TVertex vertex);
		Ref<VertexArray> Flush();

		uint32_t GetVertexCount() const
		{
			return m_vertex_count;
		}
	private:
		Ref<VertexArray> m_va;
		Ref<VertexBuffer> m_vb;
		Ref<IndexBuffer> m_ib;

		TVertex* m_vertex_buffer_base{ nullptr };
		TVertex* m_vertex_buffer_ptr{ nullptr };

		uint32_t m_vertex_count{ 0 };

	};

}

#include "Batch_impl.h"
