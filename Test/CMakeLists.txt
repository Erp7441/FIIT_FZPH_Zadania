cmake_minimum_required(VERSION 3.25.1)
project(Test C)

set(CMAKE_C_STANDARD 11)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(Test main.c)

target_link_libraries(Test PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)