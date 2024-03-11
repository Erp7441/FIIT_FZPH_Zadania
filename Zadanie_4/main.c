
#include "window.h"
#include "utils.h"
#include "shapes.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define FPS 360
//#define DEBUG

void draw();
void update(int i);
void get_data();
bool collision(float x1, float y1, float x2, float y2, float r1, float r2);

// Input parameters
float angle = 0.f;  // 45
float distance = 0.f;  // 250 metrov

// Global variables
float angle_rad = 0.f;

// Package
float pack_s_x = 1.f;
float pack_s_y = 0.f;
float pack_x = 1.f;
float pack_y = 0.f;
float pack_size = 0.1f;

// Bullet
float bullet_s_x = -1.65f;
float bullet_s_y = -0.9f;
float bullet_x = 0.f;
float bullet_y = 0.f;
float bullet_size = 0.05f;
float bullet_vel = 7.f;


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

    // Bullet
    float b_v0_x = bullet_vel * cosf(angle_rad);
    float b_v0_y = bullet_vel * sinf(angle_rad);

    bullet_x = bullet_s_x + b_v0_x * time;
    bullet_y = bullet_s_y + b_v0_y * time - 0.5f * G * (time * time);

    #ifdef DEBUG
        printf("b_v0x: %f \t b_v0y: %f \t bullet_x: %f \t bullet_y: %f\n", b_v0_x, b_v0_y, bullet_x, bullet_y);
    #endif

    // Package
    #ifdef PACK_X
        pack_x = distance * cosf(angle_rad);
    #endif
    pack_y = pack_s_y + (distance * sinf(angle_rad)) - 0.5f * G * (time * time);

    #ifdef DEBUG
        printf("pack_x: %f \t pack_y: %f \t vel: %f \t angle: %f\n", pack_x, pack_y, bullet_vel, angle_rad);
    #endif

    if (collision(bullet_x, bullet_y, pack_x, pack_y, bullet_size, pack_size))
    {
        #ifdef DEBUG
            printf("Collision\n");
        #endif

        printf("\nPress any key to exit...\n");
        getchar();
        glutLeaveMainLoop();
    }

    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{
    printf("Zadajte uhol v stupnoch:\n");
    scanf("%f", &angle);
    getchar();

    distance = get_meters("Zadajte vzdialenost:\n");

    angle_rad = angle * (float)(M_PI / 180.f);

    printf("Press any key to start...\n");
    getchar();
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

bool collision(float x1, float y1, float x2, float y2, float d1, float d2)
{
    float distX = x1 - x2;
    float distY = y1 - y2;
    float dist = sqrtf((distX * distX) + (distY * distY));
    #ifdef DEBUG
        printf("Dist: %f \t D1: %f \t D2: %f \t D1+D2: %f\n", dist, d1, d2, d1 + d2);
    #endif
    return dist <= (d1 + d2) / 2;
}