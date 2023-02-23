#pragma once

#include <glm/glm.hpp>

#include "Batch2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer2D.h"
#include "Engine/Core/Core.h"
#include <string>

namespace Engine
{

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		int32_t texture_slot;

		//for editor
		int32_t entity_id{ 0 };
	};
	struct CircleVertex
	{
		glm::vec3 position;
		glm::vec3 local_position;
		glm::vec4 color;
		float thickness;
		float fade;

		//for editor
		int32_t entity_id{ 0 };
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		int32_t entity_id{ 0 };
	};

	enum class Primitive
	{
		Quad, Circle, Line
	};

	template<typename TVertex>
	class Batch2D
	{
	public:
		Batch2D(const std::string& shader_path, Primitive primitive);

		~Batch2D();

		void Begin();

		void End();

		bool Add(const glm::mat4& transform, Ref<Texture> texture, const glm::vec4& color, int32_t entity_id);

		bool Add(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int32_t entity_id);

		bool Add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int32_t entity_id);

		Ref<VertexArray> Flush();

		uint32_t GetIndexCount() const
		{
			return m_index_count;
		};

	private:
		static std::array<glm::vec4, 4> s_default_quad_vertices;

	private:
		Ref<VertexArray> m_va;
		Ref<VertexBuffer> m_vb;
		Ref<IndexBuffer> m_ib;
		Ref<Shader> m_shader;
		std::array<Ref<Texture>, Renderer2D::k_max_texture_slot> m_textures;
		uint32_t m_texture_count{ 0 };

		uint32_t m_index_count{ 0 };
		TVertex* m_vertex_buffer_base{ nullptr };
		TVertex* m_vertex_buffer_ptr{ nullptr };

		Primitive m_primitive_type;

		uint32_t m_primitive_count{ 0 };
	};

} // Engine

#include"Batch2D_impl.h"
