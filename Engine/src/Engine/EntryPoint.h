#pragma once

extern  Engine::Application* CreateAppliction();

int main()
{
    LogLib::Log::Init();

    Engine::Application* app = CreateAppliction(); //how with declaretion we know member fun of app
    app->Run();
    delete app;

    return 0;
}