cmake_minimum_required(VERSION 3.23)

project(Engine)


add_library(Engine STATIC
        src/Engine/Core/Application.h
        include/Engine.h
        src/Engine/Core/Application.cpp
        src/Engine/Core/EntryPoint.h
        src/Engine/Core/Window.h
        src/Engine/Core/Log.h
        src/Engine/Core/Log.cpp
        src/Engine/Event/Event.h
        src/Engine/Event/KeyEvent.h
        src/Engine/Event/MouseEvent.h
        src/Engine/Event/ApplicationEvent.h
        src/Platform/Windows/WindowsWindow.h
        src/Platform/Windows/WindowsWindow.cpp
        src/Engine/Core/Layer.h
        src/Engine/Core/LayerStack.cpp
        src/Engine/Core/LayerStack.h
        src/Engine/Core/Core.h
        src/Engine/Core/Input.h
        src/Platform/Windows/WindowsInput.cpp
        src/Engine/Core/KeyCode.h
        src/Engine/Core/MouseButtonCode.h
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
        src/Engine/Core/TimeStep.h
        src/Platform/OpenGL/OpenGLShader.cpp
        src/Engine/ImGui/ImGuiLayer.cpp
        src/Engine/ImGui/ImGuiLayer.h
        src/Engine/Renderer/Texture.cpp
        src/Engine/Renderer/Texture.h
        src/Platform/OpenGL/OpenGLTexture2D.cpp
        src/Platform/OpenGL/OpenGLTexture2D.h
        src/Engine/Renderer/Renderer2D.cpp
        src/Engine/Renderer/Renderer2D.h
        src/Engine/Debug/Instrumentor.h
        src/Engine/Renderer/SubTexture2D.cpp
        src/Engine/Renderer/SubTexture2D.h
        src/Engine/Renderer/FrameBuffer.cpp
        src/Engine/Renderer/FrameBuffer.h
        src/Platform/OpenGL/OpenGLFrameBuffer.cpp
        src/Platform/OpenGL/OpenGLFrameBuffer.h
        src/Engine/Systems/Random.cpp
        src/Engine/Scene/Scene.h
        src/Engine/Scene/Scene.cpp
        src/Engine/Scene/Entity.h
        src/Engine/Scene/Components.h
        src/Engine/Renderer/Camera.h src/Engine/Scene/SceneCamera.h
        src/Engine/Scene/ScriptableEntity.h src/Engine/Scene/SceneSerializer.h
        src/Engine/Scene/SceneSerializer.cpp src/Engine/Utils/FileDialogs.h
        src/Platform/Windows/WindowsPlatformUtils.cpp src/Platform/Windows/WindowsPlatformUtils.cpp src/Engine/Math/Math.h src/Engine/Math/Math.cpp src/Engine/Math/Math.cpp src/Engine/Scene/EditorCamera.cpp src/Engine/Scene/EditorCamera.h src/Platform/Linux/LinuxPlatformUtils.cpp src/Engine/Core/UUID.h src/Engine/Core/UUID.cpp src/Engine/Renderer/Batch.h src/Engine/Renderer/Batch_impl.h src/Engine/Renderer/Camera.cpp
        src/Engine/Script/ScriptEngine.h src/Engine/Script/ScriptEngine.cpp)


target_include_directories(Engine
        PUBLIC include
        PUBLIC src)


target_precompile_headers(Engine PUBLIC src/pch.h)

#config
include_directories(${CMAKE_BINARY_DIR})

if (DEBUG)
    add_compile_definitions(DEBUG)
endif (DEBUG)

if (PROFILER)
    add_compile_definitions(PROFILER)
endif (PROFILER)

if (RELEASE)
    add_compile_definitions(RELEASE)
endif (RELEASE)

if (MSVC)
    add_compile_definitions(WINDOWS)
endif (MSVC)

if (UNIX AND NOT APPLE)
    add_compile_definitions(LINUX)
endif ()



