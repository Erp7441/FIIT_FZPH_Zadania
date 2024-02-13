# FZPH_Zadania
Priebezne zadania z predmetu Fyzika v pocitacovych hrach.

Ako importnut freeglut po instalacii pomocou cmakeu:
```cmake
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

...
target_link_libraries(Test PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
```
