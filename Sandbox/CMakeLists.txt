cmake_minimum_required(VERSION 3.23)

project(Sandbox)

add_executable(Sandbox src/Sandbox.cpp
        src/Sandbox2D.cpp src/Sandbox2D.h
        src/FirstGame/Quad.h
        src/FirstGame/Transform.h
        src/FirstGame/Quad.cpp
        src/FirstGame/RendererSys.h src/FirstGame/RendererSys.cpp)


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