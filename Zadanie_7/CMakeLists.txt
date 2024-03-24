cmake_minimum_required(VERSION 3.27)
project(Zadanie_7 C)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(Zadanie_7 main.c
        window.h
        shapes.h
        utils.h)


target_link_libraries(Zadanie_7 PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU m)
