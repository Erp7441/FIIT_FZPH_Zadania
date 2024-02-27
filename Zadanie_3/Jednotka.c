#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

void draw_circle(float x, float y, float z);
void draw_circles();

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
//    glutInitWindowSize(400, 400);

    // Ziskanie vstupnych paramterov od usera.
//    get_input_params();

    glutCreateWindow("OpenGL: Zadanie 2");
//    glutFullScreen();
//    start_time = glutGet(GLUT_ELAPSED_TIME);
    glutDisplayFunc(&draw_circles);

//    glutTimerFunc(TIME_STEP, update, 0);
    glutMainLoop();

    return 0;
}


void draw_circle(float x, float y, float z)
{
    glTranslatef(x, y, z);
    glBegin(GL_TRIANGLE_FAN);
    const int NUM_RAYS = 360;
    const double PI_2 = 2*M_PI;

    for(int i = 0; i < NUM_RAYS; i++)
    {
        float cos_x = (float)cos(PI_2 * i/NUM_RAYS);
        float sin_y = (float)sin(PI_2 * i/NUM_RAYS);
        glVertex2f(cos_x, sin_y);
    }
    glEnd();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (w <= h)
    {
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    }
    else
    {
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw_circles()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);  // Modra
    draw_circle(0.f, 0.f, 0.f);
    glutReshapeFunc(resize);

    glutSwapBuffers();
}
