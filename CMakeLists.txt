cmake_minimum_required(VERSION 3.23)

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


target_link_libraries(Sandbox PRIVATE Engine)
target_link_libraries(EngineEditor PRIVATE Engine)

target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glad)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC imgui)
target_link_libraries(Engine PUBLIC stb_image)

target_link_libraries(imgui PUBLIC glfw)











