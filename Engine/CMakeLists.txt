cmake_minimum_required(VERSION 3.23)

project(Engine)


add_library(Engine STATIC src/Engine/Application.h
        include/Engine.h
        src/Engine/Application.cpp
        src/Engine/EntryPoint.h
        src/Engine/Window.h
        src/Engine/Log.h
        src/Engine/Log.cpp
        src/Engine/Event/Event.h
        src/Engine/Event/KeyEvent.h
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
        src/Platform/OpenGL/OpenGLContext.h
        src/Engine/Renderer/Shader.cpp
        src/pch.h
        src/pch.cpp
        src/Platform/OpenGL/OpenGLBuffer.h
        src/Platform/OpenGL/OpenGLBuffer.cpp
        src/Engine/Renderer/Buffer.cpp
        src/Engine/Renderer/Buffer.h
        src/Engine/Renderer/Renderer.cpp
        src/Engine/Renderer/VertexArray.cpp
        src/Engine/Renderer/VertexArray.h
        src/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Platform/OpenGL/OpenGLVertexArray.h
        src/Engine/Renderer/RendererAPI.cpp
        src/Engine/Renderer/RendererAPI.h
        src/Engine/Renderer/RendererCommand.cpp
        src/Engine/Renderer/RendererCommand.h
        src/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/Platform/OpenGL/OpenGLRendererAPI.h
        src/Engine/Renderer/OrthographicCamera.cpp
        src/Engine/Core/TimeStep.h
        src/Platform/OpenGL/OpenGLShader.cpp
        src/Engine/ImGui/ImGuiLayer.cpp
        src/Engine/ImGui/ImGuiLayer.h)


target_include_directories(Engine
        PUBLIC include
        PUBLIC src)


target_precompile_headers(Engine PUBLIC src/pch.h)


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




