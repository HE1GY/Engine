#pragma once
#include <Engine.h>

namespace Scripts
{
	using namespace Engine;
	class BallController : public ScriptableEntity
	{
	public:
		ScriptableEvent<> out_of_field;
		
		virtual void OnUpdate(TimeStep ts);

		virtual void OnCollision2D(glm::vec2& normal) override;

	private:
		glm::vec3 m_velocity = { 0.7, 0.7, 0 };
		TransformComponent* m_transform_cmp;
	};
}

