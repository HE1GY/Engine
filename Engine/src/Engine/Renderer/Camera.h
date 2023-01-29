#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
				:m_projection{ projection }
		{
		}
		
		const glm::mat4& get_projection() const
		{
			return m_projection;
		}

	private:
		glm::mat4 m_projection;
	};
}