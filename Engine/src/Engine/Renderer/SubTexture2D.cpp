#include "SubTexture2D.h"

namespace Engine
{
	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& max, const glm::vec2& min)
			:m_texture{ texture }
	{
		m_coords[0] = { min.x, min.y };
		m_coords[1] = { max.x, min.y };
		m_coords[2] = { max.x, max.y };
		m_coords[3] = { min.x, max.y };
	}
	Ref<SubTexture2D>
	SubTexture2D::CreateFromCoord(Ref<Texture2D>& texture, const glm::vec2& coord_index, const glm::vec2& sprite_size,
			const glm::vec2& coord_cell_size)
	{
		glm::vec2 min = { (coord_index.x * coord_cell_size.x) / texture->get_width(),
						  (coord_index.y * coord_cell_size.y) / texture->get_height() };
		glm::vec2 max = { ((coord_index.x * coord_cell_size.x) + sprite_size.x) / texture->get_width(),
						  ((coord_index.y * coord_cell_size.y) + sprite_size.y) / texture->get_height() };
		return CreateRef<SubTexture2D>(texture, max, min);
	}

}