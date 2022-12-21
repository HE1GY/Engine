cmake_minimum_required(VERSION 3.23)

project(Engine)

#file(GLOB_RECURSE SRC_FILE *.cpp *.h)

add_library(Engine STATIC src/Engine/Application.h include/Engine.h  src/Engine/Application.cpp src/Engine/EntryPoint.h)


target_include_directories(Engine 
							PUBLIC include
							PUBLIC src)



