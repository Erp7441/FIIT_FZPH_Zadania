
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define FPS 30

void draw();
void update(int i);
void get_data();
bool collision(float x1, float y1, float x2, float y2, float r1, float r2);
void slow_down(unsigned char key, int x, int y);

// Input parameters


// Global variables
float w_0 = 3.7; // s^{-1}
float t_r = 5.f;  // s
float E = 1.f; // s{-2}
int n = 0;  // Otacky
float t_b = 0.f; // w_0 / E
float phi = 0.f;
float phi_rad = 0.f;
float r = 0.f;


// Boy
float boy_x = 0.f;
float boy_y = 0.f;
const float boy_s_x = 0.f;
const float boy_s_y = 0.f;
const float boy_size = 0.05f;

float total_angle = 0.f;
bool stopping = false;

float deacceleration = 0.f;
float deacceleration_start_time = 0.f;

FILE* data_file = NULL;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 4", 1366, 768, false);

    glutKeyboardFunc(slow_down);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void update(const int i)
{
    float time = display_frame();

    // Update logic
    float angle = fmodf(w_0 * time, 2.f*(float)M_PI);
    total_angle = w_0 * time;

    printf("Angle: %f\n", angle);

    float revolutions = total_angle / (2.f * (float)M_PI);
    n = (int)ceilf(revolutions);

    boy_x =  + r * cosf(angle);
    boy_y = boy_s_y + r * sinf(angle);

    if (stopping)
    {
        if (deacceleration_start_time == 0.f)
        {
            deacceleration_start_time = time;
        }

        deacceleration = to_meters(2 * (time - deacceleration_start_time));
        w_0 = fmaxf(0.f,  w_0 - E * deacceleration);
    }
    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{
    r = 1.f;

    boy_x = boy_s_x;
    boy_y = boy_s_y;
    t_b = w_0 / E;

    float phi_1 = w_0 * t_r;
    float phi_2 = w_0 * t_b - 1/2 * E * (t_b * t_b);
    phi = phi_1 + phi_2;

    phi_rad = phi * (M_PI / 180.f);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 0.0f);  // Zelena
    draw_circle(boy_x, boy_y, 0.f, boy_size);

    glutSwapBuffers();
}

void slow_down(unsigned char key, int x, int y)
{
    if (key == 32)
    {
        stopping = true;
    }
}