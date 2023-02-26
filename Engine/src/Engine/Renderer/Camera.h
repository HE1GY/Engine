#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	enum class ProjectionType
	{
		Perspective, Orthographic
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
				:m_projection{ projection }
		{
		}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const
		{
			return m_projection;
		}

		const std::array<glm::vec2, 2>& GetOrthographicBound() const
		{
			return m_ortho_bound;
		}

		void SetProjectionType(ProjectionType type);
		void SetOrthographic(float aspect_ration, float size, float near, float far);
		void SetPerspective(float aspect_ration, float fov, float near, float far);

	protected:
		void CalculateProjection();
	protected:
		glm::mat4 m_projection;
		ProjectionType m_projection_type{ ProjectionType::Orthographic };

		float m_aspect_ration{ 1.7f };

		//Ortho
		float m_near_ortho{ 1.0f }, m_far_ortho{ -1.0f };
		float m_ortho_size{ 1.0f };
		std::array<glm::vec2, 2> m_ortho_bound; //[0] top left; [1] down right

		//Persp
		float m_near_persp{ 0.01f }, m_far_persp{ 1000.0f };
		float m_fov{ glm::radians(45.0f) };
	};
}