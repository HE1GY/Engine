cmake_minimum_required(VERSION 3.23)

project(Engine)


add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(external/spdlog)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)


target_link_libraries(Sandbox PRIVATE Engine)
target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glad)







