#include "Engine.h"

class SandBox:public Engine::Application
{
};

Engine::Application* CreateApplication()
{
	return new SandBox();
}
