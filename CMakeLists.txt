cmake_minimum_required(VERSION 3.23)


set(CMAKE_SYSTEM_VERSION 10.0.22621.0)

project(Engine)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(EngineEditor)

add_subdirectory(external/spdlog)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/imgui)
add_subdirectory(external/stb_image)
add_subdirectory(external/entt)
add_subdirectory(external/yaml-cpp)
add_subdirectory(external/imguizmo)
add_subdirectory(external/box2d)


target_link_libraries(Sandbox PRIVATE Engine)

target_link_libraries(EngineEditor PRIVATE Engine)
target_link_libraries(EngineEditor PRIVATE imguizmo)


target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glad)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC imgui)
target_link_libraries(Engine PUBLIC stb_image)
target_link_libraries(Engine PUBLIC EnTT)
target_link_libraries(Engine PUBLIC yaml-cpp)
target_link_libraries(Engine PUBLIC imguizmo)
target_link_libraries(Engine PUBLIC box2d)

target_link_libraries(imgui PUBLIC glfw)

target_link_libraries(imguizmo PUBLIC imgui)

#config
configure_file(config.h.in ${CMAKE_BINARY_DIR}/config.h)

#precompiled
target_include_directories(Engine PUBLIC external/precompiled/include)

#script engine
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/eglib.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/libmini-sgen.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/libmonoruntime-sgen.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/libmono-static-sgen.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/libmonoutils.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/mono-2.0-sgen.lib)
target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/precompiled/lib/mono/Debug/MonoPosixHelper.lib)

#windows
target_link_libraries(Engine PUBLIC Ws2_32.lib)
target_link_libraries(Engine PUBLIC Winmm.lib)
target_link_libraries(Engine PUBLIC Version.lib)
target_link_libraries(Engine PUBLIC Bcrypt.lib)














