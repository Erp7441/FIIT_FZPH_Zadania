
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define FPS 360
//#define DEBUG

void draw();
void update(int i);
void get_data();
void slow_down(unsigned char key, int x, int y);

// Input parameters
float r = 0.f;  // Polomer kolotoca

// Global variables
float w_0 = 3.7f; // Uholova rychlost
float E = 1.f; // Rychlost spomalenia
int n = 0;  // Otacky

// Boy
float boy_x = 0.f;
float boy_y = 0.f;
const float boy_size = 0.05f;

float angle = 0.f;
bool stopping = false;

float deacceleration = 0.f;
float deacceleration_start_time = 0.f;

float prev_time = 0.f;

FILE* data = NULL;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 4", 1366, 768, false);

    data = open_file("./data.csv", "w");
    write_to_file(data, "x,y,t\n");

    glutKeyboardFunc(slow_down);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    close_file(data);

    return 0;
}

void update(const int i)
{
    float time = display_frame();

    // Update logic
    float delta_time = time - prev_time;
    prev_time = time;

    angle += w_0 * delta_time;

    float revolutions = angle / (2.f * (float)M_PI);

    #ifdef DEBUG
        if ((int)ceilf(revolutions)-1 > n)
        {
            printf_debug("Num of revolutions: %d\n", (int)ceilf(revolutions)-1);
        }
    #endif
    n = (int)ceilf(revolutions)-1;

    boy_x = r * cosf(angle);
    boy_y = r * sinf(angle);

    write_to_file(data, "%f,%f,%f\n", boy_x, boy_y, time);

    if (stopping)
    {
        if (deacceleration_start_time == 0.f)
        {
            deacceleration_start_time = time;
        }

        deacceleration = to_meters(2 * (time - deacceleration_start_time));
        w_0 = fmaxf(0.f,  w_0 - E * deacceleration);
    }

    if (stopping && w_0 == 0.f)
    {
        sleep(2);

        printf("Num of revolutions: %d\n", n);
        printf("Data saved to data.csv!\n");


        printf("Press any key to exit...");
        getchar();
        glutLeaveMainLoop();
    }
    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{
    printf("Zadajte polomer kolotoca:\n");
    scanf("%f", &r);
    getchar();

    r = to_meters(r);
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