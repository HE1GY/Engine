#include "PlatformController.h"
namespace Scripts
{
	void PlatformController::OnCollision2D(glm::vec2& normal)
	{
		Destroy();
	}
	PlatformController::~PlatformController()// not works
	{
		INFO("Platform died");
	}
	void PlatformController::OnDestroy()
	{
		INFO("Platform destroyed");
	}
}