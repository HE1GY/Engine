#include "PlatformController.h"
namespace Scripts
{
	void PlatformController::OnCollision2D(glm::vec2& normal)
	{
		Destroy();
	}

	void PlatformController::OnDestroy()
	{
		TRACE("Platform destroyed");
	}
}