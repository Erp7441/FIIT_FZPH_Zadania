
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define BUFFER_SIZE 256
#define GAME_MODE
//#define BULLET_BOUNDS

void get_data();
void draw();
// Update
void update(int i);



// Input parameters
float slope_angle = 45.f;
float slope_angle_rad = 0.f;
float engine_force = 10.f;
float quad_mass = 0.5f;
float friction = 0.5f;

float quad_x = 0.f;
float quad_y = 0.f;

float anim_time = 0.f;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 8", 1920, 1080, true);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void update(const int i)
{
    anim_time = display_frame();

    float quad_g_force = quad_mass * G;
    float force_t = friction * quad_g_force * cosf(slope_angle_rad);
    float force_n = quad_g_force * sinf(slope_angle_rad);
    float acc = (engine_force - force_n - force_t) / quad_mass;

    float x2 = 0.5f * acc * (anim_time * anim_time);

    quad_x = x2 * cosf(slope_angle_rad);
    quad_y = x2 * sinf(slope_angle_rad);

    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{

    slope_angle_rad = slope_angle * (float)M_PI / 180.f;

    printf("Press any key to start...\n");
    getchar();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);


    draw_quad(quad_x*cosf(slope_angle_rad), quad_y*sinf(slope_angle_rad), 0.f, 0.5f);

    draw_slope_line(0, 0, 100, slope_angle);

    draw_axes();


    glutSwapBuffers();
}
