cmake_minimum_required(VERSION 3.23)

project(EngineEditor)

add_executable(EngineEditor src/EngineEditorApp.cpp
src/EngineEditorLayer.cpp
src/EngineEditorLayer.h
src/Panel/SceneHierarchyPanel.cpp
src/Panel/SceneHierarchyPanel.h)


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