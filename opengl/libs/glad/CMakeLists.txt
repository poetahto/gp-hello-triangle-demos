cmake_minimum_required(VERSION 3.20)
project(GLAD)
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_SOURCE_DIR}/include)
