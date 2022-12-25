#pragma once

extern Engine::Application* CreateApplication();

int main()
{
	Engine::Log::Init();

	Engine::Application* app = CreateApplication(); //how with declaration we know member fun of app
	app->Run();
	delete app;

	return 0;
}