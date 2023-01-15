cmake_minimum_required(VERSION 3.23)

project(stb_image)

add_library(stb_image STATIC src/stb_image.cpp)
target_include_directories(stb_image PUBLIC include)