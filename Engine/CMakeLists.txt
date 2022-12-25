cmake_minimum_required(VERSION 3.23)

project(Engine)


add_library(Engine STATIC src/Engine/Application.h include/Engine.h
        src/Engine/Application.cpp
        src/Engine/EntryPoint.h
        src/Engine/Window.h
        src/Engine/Log.h
        src/Engine/Loh.cpp
        src/Engine/Event/MouseEvent.h
        src/Engine/Event/ApplicationEvent.h
        src/Platform/Windows/WindowsWindow.h
        src/Platform/Windows/WindowsWindow.cpp)


target_include_directories(Engine
        PUBLIC include
        PUBLIC src)



