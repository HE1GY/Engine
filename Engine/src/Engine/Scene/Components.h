#pragma once
#include "glm/glm.hpp"

namespace Engine
{

	struct TransformComponent
	{
		glm::mat4 transform{ 1.0f };

		operator glm::mat4() const
		{
			return transform;
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1, 1, 1, 1 };
	};

	struct TagComponent
	{
		TagComponent(const std::string& tag)
				:tag{ tag }
		{
		}
		std::string tag;
	};
}
