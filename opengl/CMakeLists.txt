cmake_minimum_required(VERSION 3.20)
project(TRIANGLE_OGL)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)

add_executable(hello_triangle_ogl main.c)
target_link_libraries(hello_triangle_ogl glad glfw)