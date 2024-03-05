#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define FPS 60
#define TIME_STEP (1000 / FPS)
#define G 9.81

// Input parameters
float start_y = 0.f;
float vel_0 = 0.f;
float t_max = 0.f;

void draw_circle(float x, float y, float z, float scale);
void draw_circles();
void update(int i);
void update_movement(float t);
void resize(int w, int h);
float get_max_height(float v0, float h0);

// Time
unsigned int start_time = 0;
unsigned int last_frame_time = 0;
unsigned int elapsed = 0;
float time_elapsed = 0.f;
float coord_y = 0.f;
float size = 0.2f;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);

    printf("Enter initial height (in meters): ");
    scanf("%f", &start_y);
    getchar();

    printf("Enter initial velocity (in meters per second): ");
    scanf("%f", &vel_0);
    getchar();

    t_max = get_max_height(vel_0, start_y);

    glutCreateWindow("Ball Simulation");
    start_time = glutGet(GLUT_ELAPSED_TIME);
    glutDisplayFunc(draw_circles);
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
    if (t <= t_max)
    {
        float val = start_y + vel_0 * t - 0.5f * G * (t * t);
        coord_y = val;
    }
    else
    {
        // Ball has hit the ground, stop updating
        glutTimerFunc(0, NULL, 0);
    }
}

void draw_circle(float x, float y, float z, float scale)
{
    glTranslatef(x, y, z);
    glBegin(GL_TRIANGLE_FAN);
    const int NUM_RAYS = 360;
    const double PI_2 = 2 * M_PI;

    for (int i = 0; i < NUM_RAYS; i++)
    {
        float cos_x = (float)cos(PI_2 * i / NUM_RAYS);
        float sin_y = (float)sin(PI_2 * i / NUM_RAYS);
        glVertex2f(cos_x * scale, sin_y * scale);
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
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    draw_circle(0.f, coord_y, 0.f, size);
    glutSwapBuffers();
}

float get_max_height(float v0, float h0)
{
    return (v0 * v0) / (2 * G) + h0; // Maximum height formula
}
