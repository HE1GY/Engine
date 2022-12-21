#pragma once
#include "Application.h"



Engine::Application* CreateAppliction();

int main()
{
    Engine::Application* app = CreateAppliction();
    app->Run();
    delete app;

    return 0;
}