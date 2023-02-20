#pragma once
#include <Engine.h>

namespace Scripts
{
	using namespace Engine;

	class PlayerController : public ScriptableEntity
	{
	public:

		virtual void OnUpdate(TimeStep ts) override;

	private:
		TransformComponent* m_transform_cmp;
	};
}