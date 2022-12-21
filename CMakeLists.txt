cmake_minimum_required(VERSION 3.23)

project(Engine)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(external/loglib)

#target_link_libraries(Engine PUBLIC Loglib)
target_link_libraries(Sandbox PRIVATE Engine)



