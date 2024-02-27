cmake_minimum_required(VERSION 3.25)
project(Zadanie_3 C)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(Zadanie_3  main.c)
target_link_libraries(Zadanie_3 PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU m)
