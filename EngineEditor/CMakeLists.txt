cmake_minimum_required(VERSION 3.23)

project(EngineEditor)

add_executable(EngineEditor src/EngineEditorApp.cpp
        src/EngineEditorLayer.cpp
        src/EngineEditorLayer.h
        src/Panel/SceneHierarchyPanel.cpp
        src/Panel/SceneHierarchyPanel.h src/Panel/ContentPanel.h src/Panel/ContentPanel.cpp)

#config
include_directories(${CMAKE_BINARY_DIR})



#script engine
add_custom_command(TARGET EngineEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
       ${CMAKE_SOURCE_DIR}/external/precompiled/bin/mono/Debug/mono-2.0-sgen.dll
       ${CMAKE_SOURCE_DIR}/external/precompiled/bin/mono/Debug/MonoPosixHelper.dll
       $<TARGET_FILE_DIR:EngineEditor>)





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