#include "BallController.h"

namespace Scripts
{
	void Scripts::BallController::OnUpdate(Engine::TimeStep ts)
	{
		m_transform_cmp = &GetComponent<TransformComponent>();
		const std::array<glm::vec2,
						 2>& camera_bound = GetMainCamera().GetComponent<CameraComponent>().camera.GetOrthographicBound();

		const float ball_speed = 0.7f;
		m_transform_cmp->translation += m_velocity * ts.GetInSeconds() * ball_speed;

		static bool wall_bounce_side{ false };
		if ((m_transform_cmp->translation.x > camera_bound[1].x - (m_transform_cmp->scale.x / 2.0f)
				&& !wall_bounce_side)
				|| (m_transform_cmp->translation.x
						< camera_bound[0].x + (m_transform_cmp->scale.x / 2.0f)
						&& !wall_bounce_side))
		{
			wall_bounce_side = true;
			m_velocity.x *= -1;
		}
		else if ((m_transform_cmp->translation.x < camera_bound[1].x - (m_transform_cmp->scale.x)
				&& wall_bounce_side)
				|| (m_transform_cmp->translation.x
						> camera_bound[0].x + (m_transform_cmp->scale.x)
						&& wall_bounce_side))
		{
			wall_bounce_side = false;
		}

		static bool wall_bounce_floor{ false };
		if ((m_transform_cmp->translation.y > camera_bound[0].y - (m_transform_cmp->scale.y / 2.0f)
				&& !wall_bounce_floor))
		{
			wall_bounce_floor = true;
			m_velocity.y *= -1;
		}
		else if ((m_transform_cmp->translation.y < camera_bound[0].y - (m_transform_cmp->scale.y)
				&& wall_bounce_floor)
				|| (m_transform_cmp->translation.y > camera_bound[1].y + (m_transform_cmp->scale.y)
						&& wall_bounce_floor))
		{
			wall_bounce_floor = false;
		}

		if ((m_transform_cmp->translation.y < camera_bound[1].y + (m_transform_cmp->scale.y / 2.0f)
				&& !wall_bounce_floor))
		{
			out_of_field.Invoke();
		}

	}
	void BallController::OnCollision2D(glm::vec2& normal)
	{
		glm::vec2 reflected = glm::reflect({ m_velocity.x, m_velocity.y }, normal);
		m_velocity = glm::vec3{ reflected.x, reflected.y, 0 };
	}
}
