# FZPH_Zadania
Priebezne zadania z predmetu Fyzika v pocitacovych hrach.

WSL:
```bash
apt install freeglut3-dev libglew-dev mesa-utils build-essential -y
```

Clion:
* Toolchain (WSL):
    * Toolset: Debian
    * Cmake: WSL CMake
    * Build tool: (auto)
    * C compiler: (auto)
    * C++ compiler: (auto)
    * Debugger: WSL GDB
* Cmake:
    * Toolchain: Use default
    * Generator: Let cmake decide

MSYS Setup:
```bash
pacman -S mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-gnuplot mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-gdb
```

Clion:
* Toolchain (MinGW):
    * Toolset: C:\tools\msys64\ucrt64
    * Cmake: Bundled
    * Build tool: (auto)
    * C compiler: gcc
    * C++ compiler: (auto)
    * Debugger: Bundled
* Cmake:
    * Toolchain: Use default
    * Generator: Use default

Ako importnut freeglut po instalacii pomocou cmakeu:
```cmake
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Zvysok konfigu az po spodok...
target_link_libraries(Test PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
```
