cmake_minimum_required(VERSION 3.27)
project(Zadanie_1 C)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(Zadanie_1 main.c)
#add_executable(Zadanie_1 debug.c)
#add_executable(Zadanie_1 collision_time.c)

target_link_libraries(Zadanie_1 PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU m)
