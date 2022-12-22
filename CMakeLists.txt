cmake_minimum_required(VERSION 3.23)

project(Engine)


add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(external/Loglib)
add_subdirectory(external/EventSys)

target_link_libraries(Engine PUBLIC LogLib)
target_link_libraries(Engine PRIVATE EventSys)
target_link_libraries(Sandbox PRIVATE Engine)




