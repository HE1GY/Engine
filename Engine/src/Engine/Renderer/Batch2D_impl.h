
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

	static std::array<glm::vec4, 4> GetDefaultQuadVertex()
	{
		std::array<glm::vec4, 4> positions;
		positions[0] = { -0.5f, -0.5f, 0, 1 };
		positions[1] = { 0.5f, -0.5f, 0, 1 };
		positions[2] = { 0.5f, 0.5f, 0, 1 };
		positions[3] = { -0.5f, 0.5f, 0, 1 };
		return positions;
	}

	template<typename TVertex>
	std::array<glm::vec4, 4> Batch2D<TVertex>::s_default_quad_vertices = GetDefaultQuadVertex();

	/////
	template<typename TVertex>
	Batch2D<TVertex>::Batch2D(const std::string& shader_path, Primitive primitive)
			:m_primitive_type{ primitive }
	{
		m_shader = Shader::Create(shader_path);
		int sampler[Renderer2D::k_max_texture_slot];
		for (int i = 0; i < Renderer2D::k_max_texture_slot; ++i)
		{
			sampler[i] = i;
		}
		m_shader->Bind();
		m_shader->SetIntArray("u_texture2D", sampler, Renderer2D::k_max_texture_slot);

		m_va = VertexArray::Create();
		m_va->Bind();

		m_vb = VertexBuffer::Create(Renderer2D::k_max_vertices * sizeof(TVertex));
		m_vb->Bind();

		BufferLayout layout;
		switch (m_primitive_type)
		{
		case Primitive::Quad:
			layout = {
					{ ShaderDataType::Float3, "a_position", },
					{ ShaderDataType::Float4, "a_color" },
					{ ShaderDataType::Float2, "a_tex_coord" },
					{ ShaderDataType::Int, "a_tex_slot", },
					{ ShaderDataType::Int, "a_entity_id" }
			};
			break;
		case Primitive::Circle:
			layout = {
					{ ShaderDataType::Float3, "a_position", },
					{ ShaderDataType::Float3, "a_local" },
					{ ShaderDataType::Float4, "a_color" },
					{ ShaderDataType::Float, "a_thickness" },
					{ ShaderDataType::Float, "a_fade", },
					{ ShaderDataType::Int, "a_entity_id" }
			};
			break;
		case Primitive::Line:
			layout = {
					{ ShaderDataType::Float3, "a_position", },
					{ ShaderDataType::Float4, "a_color" },
					{ ShaderDataType::Int, "a_entity_id" }
			};
			break;
		}

		m_vb->SetLayout(layout);
		m_va->SetVertexBuffer(m_vb);

		m_vertex_buffer_base = new TVertex[Renderer2D::k_max_vertices];

		m_ib = IndexBuffer::Create(CreateIndexBuffer(), Renderer2D::k_max_vertices);
		m_ib->Bind();
		m_va->SetIndexBuffer(m_ib);
		m_va->UnBind();
	}

	template<typename TVertex>
	Batch2D<TVertex>::~Batch2D()
	{
		delete m_vertex_buffer_base;
	}

	template<typename TVertex>
	void Batch2D<TVertex>::Begin()
	{
		m_vertex_buffer_ptr = m_vertex_buffer_base;
		m_index_count = 0;
	}

	template<typename TVertex>
	void Batch2D<TVertex>::End()
	{
		uint32_t size = (m_vertex_buffer_ptr - m_vertex_buffer_base) * sizeof(TVertex);
		m_vb->SetData(m_vertex_buffer_base, size);
	}

	template<typename TVertex>
	Ref<VertexArray> Batch2D<TVertex>::Flush()
	{
		for (int i = 0; i < m_texture_count; ++i)
		{
			m_textures[i]->Bind(i);
		}
		m_shader->Bind();
		return m_va;
	}

	template<typename TVertex>
	bool
	Batch2D<TVertex>::Add(const glm::mat4& transform, Ref<Texture> texture, const glm::vec4& color, int32_t entity_id)
	{
		if (m_index_count >= Renderer2D::k_max_indices)
			return false;

		int32_t texture_slot{ -1 };
		for (int i = 0; i < m_texture_count; ++i)
		{
			if (texture->GetRendererId() == m_textures[i]->GetRendererId())
			{
				texture_slot = i;
			}
		}
		if (texture_slot == -1)
		{
			texture_slot = (float)m_texture_count;
			m_textures[texture_slot] = texture;
			m_texture_count++;
		}

		for (int i = 0; i < 4; ++i)
		{
			m_vertex_buffer_ptr->position = transform * s_default_quad_vertices[i];
			m_vertex_buffer_ptr->color = color;
			m_vertex_buffer_ptr->tex_coord = { i > 0 && i < 3 ? 1 : 0, i > 1 ? 1 : 0 };
			m_vertex_buffer_ptr->texture_slot = texture_slot;
			m_vertex_buffer_ptr->entity_id = entity_id;
			m_vertex_buffer_ptr++;
		}

		m_index_count += 6;

		m_primitive_count++;

		return true;
	}

	template<typename TVertex>
	bool Batch2D<TVertex>::Add(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade,
			int32_t entity_id)
	{
		if (m_index_count >= Renderer2D::k_max_indices)
			return false;

		for (int i = 0; i < 4; ++i)
		{
			m_vertex_buffer_ptr->position = transform * s_default_quad_vertices[i];
			m_vertex_buffer_ptr->local_position = s_default_quad_vertices[i] * 2.0f;
			m_vertex_buffer_ptr->color = color;
			m_vertex_buffer_ptr->thickness = thickness;
			m_vertex_buffer_ptr->fade = fade;
			m_vertex_buffer_ptr->entity_id = entity_id;
			m_vertex_buffer_ptr++;
		}

		m_index_count += 6;

		m_primitive_count++;

		return true;
	}

	template<typename TVertex>
	bool Batch2D<TVertex>::Add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int32_t entity_id)
	{
		if (m_index_count >= Renderer2D::k_max_indices)
			return false;

		m_vertex_buffer_ptr->position = p0;
		m_vertex_buffer_ptr->color = color;
		m_vertex_buffer_ptr->entity_id = entity_id;
		m_vertex_buffer_ptr++;

		m_vertex_buffer_ptr->position = p1;
		m_vertex_buffer_ptr->color = color;
		m_vertex_buffer_ptr->entity_id = entity_id;
		m_vertex_buffer_ptr++;

		m_index_count += 2;

		return true;
	}

} 