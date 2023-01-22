cmake_minimum_required(VERSION 3.23)

project(Sandbox)

add_executable(Sandbox src/Sandbox.cpp
        src/Sandbox2D.cpp src/Sandbox2D.h)


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