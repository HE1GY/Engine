cmake_minimum_required(VERSION 3.23)

project(Engine)


add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(external/spdlog)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/imgui)
add_subdirectory(external/stb_image)


target_link_libraries(Sandbox PRIVATE Engine)
target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glad)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC imgui)
target_link_libraries(Engine PUBLIC stb_image)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
endif ()







