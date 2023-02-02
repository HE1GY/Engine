#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
				:m_projection{ projection }
		{
		}
		virtual ~Camera() = default;

		const glm::mat4& get_projection() const
		{
			return m_projection;
		}

	protected:
		glm::mat4 m_projection;
	};
}