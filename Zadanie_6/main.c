
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

#define FPS 60

// Function definitions
void draw();
void update(int i);
void get_data();
bool collision(float x1, float y1, float x2, float y2, float r1, float r2);
void key_handler(unsigned char key, int x, int y);
void check_bounds(float* value, float offset, float bound);
float calc_distance(float x1, float y1, float x2, float y2);
float calc_angle(float x1, float y1, float x2, float y2);
void reset_game(bool hard_reset);
void detach_package();
void close_game();
void update_package();

// Input parameters
float angle = 0.f;  // 45 stupnov
//float distance = 0.f;  // 250 metrov  // TODO::Remove?

float time = 0.f;
float detach_time = 0.f;

// Global variables
float angle_rad = 0.f;

// Drone
float drone_x = 0.f;
float drone_y = 0.f;
float curr_dist = 0.f;
float curr_angle = 0.f;
const float drone_s_x = 1.5f;
const float drone_s_y = 0.5f;
const float drone_size = 0.08f;

// Package
float pack_x = 1.f;
float pack_y = 0.f;
float pack_s_x = 1.f;
float pack_s_y = 0.f;
bool pack_attached = true;
const float pack_size = 0.05f;
const float pack_att_offset_x = 0.0f;
const float pack_att_offset_y = 0.1f;

// Bullet
float bullet_x = 0.f;
float bullet_y = 0.f;
float bullet_vel = 7.f;
const float bullet_s_x = -1.65f;
const float bullet_s_y = -0.9f;
const float bullet_size = 0.05f;

// Window bounds
const float window_x_bound = 1.77f;
const float window_y_bound = 1.f;

// Game
bool auto_aim = false;  // Auto aim flag
bool game_end = false;  // Game end flag

// TODO:: separate files for rounds?
FILE* data_file = NULL;  // CSV file containing data

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 4", 1366, 768, false);

    data_file = open_file("data.csv", "w");
    write_to_file(data_file, "bullet_x,bullet_y,package_x,package_y\n");

    glutKeyboardFunc(key_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void get_data()
{
    auto_aim = get_confirmation("Zapnut auto-aim? (y/n)\n");

    if (!auto_aim)
    {
        printf("Zadajte uhol v stupnoch:\n");
        scanf("%f", &angle);
        getchar();
    }

//    distance = get_meters("Zadajte vzdialenost:\n");  // TODO:: Remove

    angle_rad = angle * (float)(M_PI / 180.f);

    bullet_x = bullet_s_x;
    bullet_y = bullet_s_y;
    drone_x = drone_s_x;
    drone_y = drone_s_y;

    printf("Press any key to start...\n");
    getchar();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 0.0f);  // Zelena
    draw_circle(pack_x, pack_y, 0.f, pack_size);

    glColor3f(0.0f, 0.0f, 1.0f);  // Modra
    draw_circle(drone_x, drone_y, 0.f, drone_size);

    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    draw_circle(bullet_x, bullet_y, 0.f, bullet_size);

    glutSwapBuffers();
}

void update(const int i)
{
    time = display_frame();

    // TODO:: Resetting after game end lags
    // At game end we await user input
    if (game_end)
    {
        glutTimerFunc(TIME_STEP, update, i + 1);
        return;
    }

    // Update logic
    // Package
    update_package();

    // Distance and angle update
    if (!game_end)
    {
        if (curr_dist != calc_distance(bullet_x, bullet_y, pack_x, pack_y))
            printf_debug("Distance: %f\n", curr_dist);
        if (curr_angle != calc_angle(bullet_x, bullet_y, pack_x, pack_y))
            printf_debug("Angle: %f\n", curr_angle);
        curr_dist = calc_distance(bullet_x, bullet_y, pack_x, pack_y);
        curr_angle = calc_angle(bullet_x, bullet_y, pack_x, pack_y);
    }

    // Check collision
    if (!game_end && collision(bullet_x, bullet_y, pack_x, pack_y, bullet_size, pack_size))
    {
        game_end = true;
        printf("\nBod stretu:\nbullet_x=%f\nbullet_y=%f\npack_x=%f\npack_y=%f\n\n", bullet_x, bullet_y, pack_x,
                     pack_y);
        printf("\nr - Reset ( s momentalnou poziciou dronu )\nR - reset\nesc - Ukoncit\n");
    }

    write_to_file(data_file, "%f,%f,%f,%f\n", bullet_x, bullet_y, pack_x, pack_y);
    glutTimerFunc(TIME_STEP, update, i + 1);
}

// Helper function for updating package position
void update_package()
{
    if (!pack_attached)
    {
        // Package fall logic
        float delta = time - detach_time;

        // pack_y = pack_s_y + (distance * sinf(angle_rad))  - 0.5f * G * (delta * delta);
        pack_y = pack_s_y  - 0.5f * G * (delta * delta);

        // Bullet update logic
        float b_v0_x = bullet_vel * cosf(angle_rad);
        float b_v0_y = bullet_vel * sinf(angle_rad);

        bullet_x = bullet_s_x + b_v0_x * delta;
        bullet_y = bullet_s_y + b_v0_y * delta - 0.5f * G * (delta * delta);
        check_bounds(&bullet_x, bullet_size, window_x_bound);
        check_bounds(&bullet_y, bullet_size, window_y_bound);

    }
    else
    {
        // Package attached logic
        pack_x = drone_x + pack_att_offset_x;
        pack_y = drone_y - pack_att_offset_y;
    }
    check_bounds(&pack_x, pack_size, window_x_bound);
    check_bounds(&pack_y, pack_size, window_y_bound);
}


// Collision detection
bool collision(float x1, float y1, float x2, float y2, float d1, float d2)
{
    float distX = x1 - x2;
    float distY = y1 - y2;
    float dist = sqrtf((distX * distX) + (distY * distY));
    return dist <= (d1 + d2) / 2;
}

void check_bounds(float* value, float offset, float bound)
{
    if (*value+offset >= bound) *value = bound - offset;
    else if (*value-offset <= -bound) *value = -bound + offset;
}


// Keyboard
void key_handler(unsigned char key, int x, int y)
{
//    printf_debug("Pressed: '%c' (%d)\n", key, key);
    switch (key)
    {
        case 'w':
            drone_y += to_meters(1.f);
            check_bounds(&drone_y, drone_size, window_y_bound);
            break;
        case 'a':
            drone_x -= to_meters(1.f);
            check_bounds(&drone_x, drone_size, window_x_bound);
            break;
        case 's':
            drone_y -= to_meters(1.f);
            check_bounds(&drone_y, drone_size, window_y_bound);
            break;
        case 'd':
            drone_x += to_meters(1.f);
            check_bounds(&drone_x, drone_size, window_x_bound);
            break;
        case ' ':
            detach_package();
            break;
        case 'r':
            reset_game(false);
            break;
        case 'R':
            reset_game(true);
            break;
        case 27: // Esc
            close_game();
            break;
        default:
            break;
    }
}


// Calcs
float calc_distance(float x1, float y1, float x2, float y2)
{
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float calc_angle(float x1, float y1, float x2, float y2)
{
    return atan2f(y2 - y1, x2 - x1) * (180.f / (float)M_PI);
}


// Utility
void reset_game(bool hard_reset)
{
    game_end = false;
    pack_attached = true;
    bullet_x = bullet_s_x;
    bullet_y = bullet_s_y;
    if (!hard_reset) return;
    drone_x = drone_s_x;
    drone_y = drone_s_y;
    time = 0.f;
    detach_time = 0.f;
    curr_dist = 0.f;
    curr_angle = 0.f;
}

void detach_package()
{
    if (!pack_attached) return;
    pack_s_x = pack_x;
    pack_s_y = pack_y;
    detach_time = time;
    pack_attached = false;

    angle_rad = curr_angle * (float)(M_PI / 180.f); // TODO:: If autoaim
}

void close_game()
{
    printf("Simulacia ukoncena!\n");

    close_file(data_file);
    printf("Data zapisane do subora!\n");

    glutLeaveMainLoop();
}
