#include "PlayerController.h"
namespace Scripts
{
	void Scripts::PlayerController::OnUpdate(TimeStep ts)
	{
		m_transform_cmp = &GetComponent<TransformComponent>();
		const std::array<glm::vec2,
						 2>& camera_bound = GetMainCamera().GetComponent<CameraComponent>().camera.GetOrthographicBound();

		const float player_speed = 1;
		if (Input::IsKeyPress(KeyCode::E_KEY_A))
		{
			m_transform_cmp->translation.x -= player_speed * ts;
		}
		else if (Input::IsKeyPress(KeyCode::E_KEY_D))
		{
			m_transform_cmp->translation.x += player_speed * ts;
		}
		m_transform_cmp->translation.x = std::clamp(m_transform_cmp->translation.x,
				(camera_bound[0].x) + (m_transform_cmp->scale.x / 2.0f),
				(camera_bound[1].x) - (m_transform_cmp->scale.x / 2.0f));

	}
}
