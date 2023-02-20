#pragma once
#include <Engine.h>

namespace Scripts
{
	using namespace Engine;

	class PlatformController : public ScriptableEntity
	{
	public:
		virtual void OnDestroy() override;
		virtual void OnCollision2D(glm::vec2& normal) override;

	private:

	};

}
