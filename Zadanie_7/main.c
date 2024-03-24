
#include "window.h"
#include "utils.h"
#include "shapes.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define G 9.81

void draw();
void update(int i);
void get_data();

// Input parameters
float start_y = 0.5f;
float vel_0 = 20.f;

// Global variables
float coord_y = 0.f;
float size = 0.2f;


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 4");

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void update(const int i)
{

}

void get_data()
{
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //glColor3f(0.0f, 1.0f, 0.0f);  // Zelena
    draw_quad_3d(0.f, 0.f, 0.f);

    glutSwapBuffers();
}

