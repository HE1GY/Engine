#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& out_translation, glm::vec3& out_rotation,
				glm::vec3& scale);
	}
}
