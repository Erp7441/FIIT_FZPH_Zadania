// Zbierat data y a cas
// Zrobit graf napr. v gnuplot
// Hodis loptu do vzduchu, simulacia dopadu

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define FPS 240
#define TIME_STEP (1000 / FPS)

void draw_circle(float x, float y, float z);
void draw_circles();
void update(int i);
void update_movement(float t);
void resize(int w, int h);

// Cas
unsigned int start_time = 0;
unsigned int last_frame_time = 0;
unsigned int elapsed = 0;  // Cas framu
float time_elapsed = 0.f;  // Celkovy cas

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1366, 768);


    glutCreateWindow("OpenGL: Zadanie 3");
    glutDisplayFunc(&draw_circles);

    glutTimerFunc(TIME_STEP, update, 0);
    glutReshapeFunc(resize);
    glutMainLoop();

    return 0;
}

void update(const int i)
{
    unsigned int current_time = glutGet(GLUT_ELAPSED_TIME);
    time_elapsed = (float)(current_time - start_time) / 1000.0f;

    elapsed = current_time - last_frame_time;

    if (elapsed > TIME_STEP)
    {
        last_frame_time = current_time - (elapsed % TIME_STEP);
        glutPostRedisplay();
    }

    update_movement(time_elapsed);
    glutTimerFunc(TIME_STEP, update, i + 1);

}

void update_movement(float t)
{

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

void resize(int w, int h) {
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

    glutSwapBuffers();
}
