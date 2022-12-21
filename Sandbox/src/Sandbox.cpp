#include "Engine.h"



class SandBox:public Engine::Application
{
};

Engine::Application* CreateAppliction()
{
	return new SandBox();
}
