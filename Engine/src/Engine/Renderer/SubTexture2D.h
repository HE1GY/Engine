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
		static Ref<SubTexture2D> CreateFromCoord(Ref<Texture2D>& texture, const glm::vec2& max, const glm::vec2& min);

	private:
		Ref<Texture2D> m_texture;
		glm::vec2 m_coords[4];
	};
}

