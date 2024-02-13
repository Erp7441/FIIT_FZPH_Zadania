# FZPH_Zadania

```cmake
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

...
target_link_libraries(Test PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
```
