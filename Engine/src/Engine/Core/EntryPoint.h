#pragma once

#include "Engine/Core/Application.h"

extern Engine::Application* CreateApplication();

int main()
{
	PROFILER_BEGIN_SESSION("Setup", "../../../Sandbox/profiler_data/setup.json");
	Engine::Log::Init();
	Engine::Application* app = CreateApplication();
	PROFILER_END_SESSION();

	PROFILER_BEGIN_SESSION("Run", "../../../Sandbox/profiler_data/run.json");
	app->Run();
	PROFILER_END_SESSION();

	PROFILER_BEGIN_SESSION("Run", "../../../Sandbox/profiler_data/shutdown.json");
	delete app;
	PROFILER_END_SESSION();
	return 0;
}