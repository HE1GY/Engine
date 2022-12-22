cmake_minimum_required(VERSION 3.23)

project(Engine)

add_subdirectory(external/loglib)
add_subdirectory(Engine)
add_subdirectory(Sandbox)


target_link_libraries(Engine PUBLIC LogLib)
target_link_libraries(Sandbox PRIVATE Engine)


