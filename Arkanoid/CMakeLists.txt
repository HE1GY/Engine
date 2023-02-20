cmake_minimum_required(VERSION 3.23)

project(Arkanoid)


add_executable(Arkanoid
        src/ArkanoidApp.cpp
        src/ArkanoidLayer.h
        src/ArkanoidLayer.cpp
        scripts/PlayerController.cpp
        scripts/BallController.cpp
        scripts/PlayerController.h
        scripts/BallController.h scripts/PlatformController.h scripts/PlatformController.cpp)

target_include_directories(Arkanoid PRIVATE scripts)


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