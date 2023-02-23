#pragma once

#include <Engine/Core/Core.h>
#include <glm/vec2.hpp>
#include "Texture.h"

namespace Engine
{
	class SubTexture2D
	{
	public:
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& max, const glm::vec2& min);
		static Ref<SubTexture2D>
		CreateFromCoord(Ref<Texture2D>& texture, const glm::vec2& coord_index, const glm::vec2& sprite_size,
				const glm::vec2& coord_cell_size);

		const glm::vec2* get_coords() const
		{
			return m_coords;
		};
		Ref<Texture2D> get_texture() const
		{
			return m_texture;
		};
	private:
		Ref<Texture2D> m_texture;
		glm::vec2 m_coords[4];
	};
}

