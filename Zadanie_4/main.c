
#include "window.h"
#include "utils.h"
#include "shapes.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>


#define FPS 5

void draw();
void update(int i);
void get_data();

// Input parameters
const float angle = 15.0f;
float angle_rad = angle * (float)(M_PI / 180.f);

// Global variables

// Package
float pack_x = 1.f;
float pack_y = 0.f;
float pack_size = 0.1f;

// Bullet
float bullet_s_x = -1.65f;
float bullet_s_y = -0.9f;
float bullet_x = 0.f;
float bullet_y = 0.f;
float bullet_size = 0.05f;
float bullet_vel = 0.05f;


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 4", 1366, 768, false);

    bullet_x = bullet_s_x;
    bullet_y = bullet_s_y;

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
//    float val = (float)  (start_y + vel_0 * time_elapsed - 0.5f * G * (time_elapsed * time_elapsed));
//
//    pack_y = to_meters(val);

//    printf("%f\n", pack_x);


    // Bullet
    float b_v0_x = bullet_vel * cosf(angle_rad);
    float b_v0_y = bullet_vel * sinf(angle_rad);

    bullet_x = bullet_s_x + b_v0_x * time;
    float part = to_meters(0.5f * G * (time * time));
    bullet_y = bullet_s_y + b_v0_y * time;

    printf("Bullet_y: %f\n", bullet_y);
    printf("b_v0_y * time: %f\n", b_v0_y * time);
    printf("Calc: %f\n",bullet_y - part);

    if (bullet_x == pack_x && bullet_y == pack_y)
    {
        printf("Collision\n");
        glutLeaveMainLoop();
    }

    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{

}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 0.0f);  // Zelena
    draw_circle(pack_x, pack_y, 0.f, pack_size);

    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    draw_circle(bullet_x, bullet_y, 0.f, bullet_size);

    glutSwapBuffers();
}

