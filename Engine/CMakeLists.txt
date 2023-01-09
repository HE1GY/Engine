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
        src/Platform/Windows/WindowsWindow.cpp
        src/Engine/Layer.h
        src/Engine/LayerStack.cpp
        src/Engine/LayerStack.h
        src/Engine/Core.h
        src/Engine/Input.h
        src/Platform/Windows/WindowsInput.cpp
        src/Platform/Windows/WindowsInput.h
        src/Engine/KeyCode.h
        src/Engine/MouseButtonCode.h
        src/Platform/OpenGL/OpenGLContext.cpp
        src/Platform/OpenGL/OpenGLContext.h src/Engine/Renderer/Shader.cpp)


target_include_directories(Engine
        PUBLIC include
        PUBLIC src)


if (DEBUG)
    add_compile_definitions(DEBUG)
endif (DEBUG)

if (RELEASE)
    add_compile_definitions(RELEASE)
endif (RELEASE)

if (MSVC)
    add_compile_definitions(WINDOWS)
endif (MSVC)

if (UNIX AND NOT APPLE)
    add_compile_definitions(LINUX)
endif ()




