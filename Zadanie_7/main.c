
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
//#define BULLET_BOUNDS

void get_data();
void draw();
// Update
void update(int i);
void update_bullet(float t);
void update_package();
// Collision
bool collision(float x1, float y1, float z1, float x2, float y2, float z2, float d1, float d2);
void check_bounds(float* value, float offset, float max_bound, float min_bound);
// Keyboard
void key_handler(unsigned char key, int x, int y);
void special_key_handler(int key, int x, int y);
// Mouse
void mouse_wheel_handler(int wheel, int direction, int x, int y);
// Calcs
float calc_distance(float x1, float y1, float z1, float x2, float y2, float z2);
float calc_azimuth(float x1, float y1, float z1, float x2, float y2, float z2);
float calc_elevation(float x1, float y1, float z1, float x2, float y2, float z2);
float calc_velocity(float dist, float height, float elev);
// Utility
void reset_game(bool hard_reset);
void reset_camera();
void detach_package();
void close_game();
char* generate_file_path(char* path);



// Input parameters
float azimuth = 0.f;  // 45 stupnov
float elevation = 0.f; // 45 stupnov

// Time
float anim_time = 0.f;
float detach_time = 0.f;

// Drone
float drone_x = 0.f;
float drone_y = 0.f;
float drone_z = 0.f;
float curr_dist = 0.f;
float curr_azimuth = 0.f;
float curr_elevation = 0.f;
float azimuth_rad = 0.f;
float elevation_rad = 0.f;
const float drone_s_x = 1.5f;
const float drone_s_y = 0.5f;
const float drone_s_z = 0.f;
const float drone_size = 0.08f;

// Package
float pack_x = 1.f;
float pack_y = 0.f;
float pack_z = 0.f;
float pack_s_x = 1.f;
float pack_s_z = 0.f;
float pack_s_y = 0.f;
bool pack_attached = true;
const float pack_size = 0.05f;
const float pack_att_offset_x = 0.0f;
const float pack_att_offset_y = 0.1f;
const float pack_att_offset_z = 0.0f;

// Bullet
float bullet_x = 0.f;
float bullet_y = 0.f;
float bullet_z = 0.f;
float bullet_vel = 7.f;
float bullet_min_vel = 7.f;
const float bullet_s_x = 0.f;
const float bullet_s_y = 0.f;
const float bullet_s_z = 0.f;
const float bullet_size = 0.05f;

// Window bounds
const float bound_max_x = 2.51f;
const float bound_max_y = 1.f;
const float bound_max_z = 2.51f;
const float bound_min_x = 0.f;
const float bound_min_y = 0.f;
const float bound_min_z = 0.f;


// Game
bool auto_aim = false;
bool game_end = false;

// File
char data_header[] = "bullet_x,bullet_y,bullet_z,package_x,package_y,package_z,time\n";
FILE* data_file = NULL;  // CSV file containing data

// Camera
float angle_horizontal = 3.9f;
float angle_horizontal_s = 3.9f;

float angle_vertical = 1.4f;
float angle_vertical_s = 1.4f;

float cam_speed = 0.1f;
float cam_distance = 2.85f;
float cam_distance_s = 2.85f;

float cam_x = 0.f;
float cam_y = 0.f;
float cam_z = 0.f;

float cam_center_x = 0.f;
float cam_center_y = 0.f;
float cam_center_z = 0.f;
float cam_center_s_x = 0.f;
float cam_center_s_y = 0.f;
float cam_center_s_z = 0.f;

float cam_up_x = 0.f;
float cam_up_y = 1.f;
float cam_up_z = 0.f;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 7");

    glutKeyboardFunc(key_handler);
    glutSpecialFunc(special_key_handler);
    glutMouseWheelFunc(mouse_wheel_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void update(const int i)
{
    anim_time = display_frame();

    // At game end we await user input
    if (game_end)
    {
        glutTimerFunc(TIME_STEP, update, i + 1);
        return;
    }

    // Update logic
    // Package
    update_package();

    // Bullet
    if (!pack_attached) update_bullet(anim_time - detach_time);

    // Distance and angle update
    if (!game_end)
    {
        if (curr_dist != calc_distance(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z))
            printf_debug("Distance: %f\n", curr_dist);
        if (curr_azimuth != calc_azimuth(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z))
            printf_debug("Azimuth: %f\n", curr_azimuth);
        if (curr_elevation != calc_elevation(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z))
            printf_debug("Elevation: %f\n", curr_elevation);

        curr_dist = calc_distance(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z);
        curr_azimuth = calc_azimuth(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z);
        curr_elevation = calc_elevation(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z);

        if (pack_attached)
        {
            bullet_vel = calc_velocity(curr_dist, (pack_y - bullet_y), curr_elevation);
        }
    }

    // Check collision
    if (!game_end && collision(bullet_x, bullet_y, bullet_z, pack_x, pack_y, pack_z, bullet_size, pack_size))
    {
        game_end = true;
        printf("\nBod stretu:\nbullet_x=%f\nbullet_y=%f\npack_x=%f\npack_y=%f\n\n", bullet_x, bullet_y, pack_x, pack_y);
        printf("\nr - Reset ( s momentalnou poziciou dronu )\nR - reset\nG - reset s novym uhlom\nesc - Ukoncit\n");
    }

    if (!game_end) write_to_file(data_file, "%f,%f,%f,%f,%f,%f,%f\n", bullet_x, bullet_y, pack_x, pack_y, anim_time);
    glutTimerFunc(TIME_STEP, update, i + 1);
}

void update_package()
{
    if (!pack_attached)
    {
        // Package fall logic
        float delta = anim_time - detach_time;
        pack_y = pack_s_y  - 0.5f * G * (delta * delta);
    }
    else
    {
        // Package attached logic
        pack_x = drone_x + pack_att_offset_x;
        pack_y = drone_y - pack_att_offset_y;
        pack_z = drone_z + pack_att_offset_z;
    }

    check_bounds(&pack_x, pack_size, bound_max_x, bound_min_x);
    check_bounds(&pack_y, pack_size, bound_max_y, bound_min_y);
    check_bounds(&pack_z, pack_size, bound_max_z, bound_min_z);
}

void update_bullet(float t)
{
    float b_v0_x = bullet_vel * cosf(elevation_rad) * cosf(azimuth_rad);
    float b_v0_y = bullet_vel * cosf(elevation_rad) * sinf(azimuth_rad);
    float b_v0_z = bullet_vel * sinf(elevation_rad);

    bullet_x = bullet_s_x + b_v0_x * t;
    bullet_y = bullet_s_y + b_v0_y * t - 0.5f * G * (t * t);
    bullet_z = bullet_s_z + b_v0_z * t;

#ifdef BULLET_BOUNDS
    check_bounds(&bullet_x, bullet_size, bound_max_x, bound_min_x);
    check_bounds(&bullet_y, bullet_size, bound_max_y, bound_min_y);
    check_bounds(&bullet_z, bullet_size, bound_max_z, bound_min_z);
#endif
}

void get_data()
{
    auto_aim = get_confirmation("Zapnut auto-aim? (y/n)\n");

    if (!auto_aim)
    {
        printf("Zadajte uhol azimutu v stupnoch:\n");
        scanf("%f", &azimuth);
        getchar();

        printf("Zadajte uhol elevacie v stupnoch:\n");
        scanf("%f", &elevation);
        getchar();
    }

    azimuth_rad = azimuth * (float)(M_PI / 180.f);
    elevation_rad = elevation * (float)(M_PI / 180.f);

    bullet_x = bullet_s_x;
    bullet_y = bullet_s_y;
    bullet_z = bullet_s_z;
    drone_x = drone_s_x;
    drone_y = drone_s_y;
    drone_z = drone_s_z;


    data_file = open_file(generate_file_path("../data.csv"), "w");
    write_to_file(data_file, data_header);


    printf("Press any key to start...\n");
    getchar();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 10.0);

    cam_x = sinf(angle_vertical) * cosf(angle_horizontal) * cam_distance;
    cam_y = cosf(angle_vertical) * cam_distance;
    cam_z = sinf(angle_vertical) * sinf(angle_horizontal) * cam_distance;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        cam_x, cam_y, cam_z,
        cam_center_x, cam_center_y, cam_center_z,
        cam_up_x, cam_up_y, cam_up_z
    );

    draw_axes(100.0f, 0.f, 0.f, 0.f);

    glColor3f(0.3f, 0.5f, 0.1f);
    draw_cube(2.51f, 0.f, 2.51f, 5.f, 0.f, 5.f);
    draw_cube_edges(2.51f, 1.f, 2.51f, 2.51f, 1.f, 2.51f);

    draw_ball_fade(pack_x, pack_y, pack_z, pack_size, 0.f, 1.f, 0.f);
    draw_ball_fade(drone_x, drone_y, drone_z, drone_size, 0.f, 0.f, 1.f);
    draw_ball_fade(bullet_x, bullet_y, bullet_z, bullet_size, 1.f, 0.f, 0.f);

    glutSwapBuffers();
}


void key_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            drone_y += to_meters(1.f);
            check_bounds(&drone_y, drone_size, bound_max_y, bound_min_y);
            break;
        case 'a':
            drone_x += to_meters(1.f);
            check_bounds(&drone_x, drone_size, bound_max_x, bound_min_x);
            break;
        case 's':
            drone_y -= to_meters(1.f);
            check_bounds(&drone_y, drone_size, bound_max_y, bound_min_y);
            break;
        case 'd':
            drone_x -= to_meters(1.f);
            check_bounds(&drone_x, drone_size, bound_max_x, bound_min_x);
            break;
        case 'q':
            drone_z -= to_meters(1.f);
            check_bounds(&drone_z, drone_size, bound_max_z, bound_min_z);
            break;
        case 'e':
            drone_z += to_meters(1.f);
            check_bounds(&drone_z, drone_size, bound_max_z, bound_min_z);
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
        case 'G':
            get_data();
            reset_game(true);
            break;
        case 'C':
            reset_camera();
            break;
        case 27: // Esc
            close_game();
            break;
        default:
            break;
    }
}

void special_key_handler(int key, int x, int y)
{
    int modifiers = glutGetModifiers();
    switch (key)
    {
        case GLUT_KEY_RIGHT:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam_center_x -= cam_speed;
            else angle_horizontal -= cam_speed;
            break;
        case GLUT_KEY_LEFT:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam_center_x += cam_speed;
            else angle_horizontal += cam_speed;
            break;
        case GLUT_KEY_UP:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam_center_z -= cam_speed;
            else angle_vertical += cam_speed;
            break;
        case GLUT_KEY_DOWN:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam_center_z += cam_speed;
            else angle_vertical -= cam_speed;
            break;
        default:
            break;
    }
}

void mouse_wheel_handler(int wheel, int direction, int x, int y)
{
    if (direction > 0)
    {
        cam_distance *= 1.1f;
    }
    else
    {
        cam_distance /= 1.1f;
    }

    glutPostRedisplay();
}

void check_bounds(float* value, float offset, float max_bound, float min_bound)
{
    if (*value+offset >= max_bound) *value = max_bound - offset;
    else if (*value+offset <= min_bound) *value = min_bound + offset;
}

// Collision
bool collision(float x1, float y1, float z1, float x2, float y2, float z2, float d1, float d2)
{
    float distX = x1 - x2;
    float distY = y1 - y2;
    float distZ = z1 - z2;
    float dist = sqrtf((distX * distX) + (distY * distY) + (distZ * distZ));
    return dist <= (d1 + d2) / 2;
}

// Calcs
float calc_distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

float calc_azimuth(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return atan2f(dy, dx) * (180.f / (float)M_PI);
}

float calc_elevation(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return atan2f(dz, sqrtf(dx * dx + dy * dy)) * (180.f / (float)M_PI);
}

float calc_velocity(float dist, float height, float elev)
{
    return 7.f;
    // TODO:: Figure out or set to fixed value

    printf("curr_dist=%f\n", curr_dist);
    printf("curr_elevation=%f\n", curr_elevation);
    printf("\nv=%f\n", bullet_vel);

    const float vel_factor = 0.5f; // adjust this value as needed

    float n = G * dist;
    float d = sinf(2 * (elev * (float)M_PI / 180.f)) + (2 * G * height);

    if (d == 0) return bullet_min_vel;

    float vel = sqrtf(n / d) + vel_factor * dist; // add term that increases with dist
    if (vel <= bullet_min_vel) vel = bullet_min_vel;
    return vel;
}

// Utility
void reset_game(bool hard_reset)
{
    game_end = false;
    write_to_file(data_file, data_header);
    pack_attached = true;
    bullet_x = bullet_s_x;
    bullet_y = bullet_s_y;
    bullet_z = bullet_s_z;

    if (!hard_reset) return;
    drone_x = drone_s_x;
    drone_y = drone_s_y;
    drone_z = drone_s_z;

    reset_camera();

    anim_time = 0.f;
    detach_time = 0.f;
    curr_dist = 0.f;
    curr_azimuth = 0.f;
    curr_elevation = 0.f;
}

void reset_camera()
{
    cam_center_x = cam_center_s_x;
    cam_center_y = cam_center_s_y;
    cam_center_z = cam_center_s_z;
    cam_distance = cam_distance_s;
    angle_horizontal = angle_horizontal_s;
    angle_vertical = angle_vertical_s;
}

void detach_package()
{
    if (!pack_attached) return;
    pack_s_x = pack_x;
    pack_s_y = pack_y;
    pack_s_z = pack_z;
    detach_time = anim_time;
    pack_attached = false;

    // Auto aiming
    if (auto_aim)
    {
        azimuth_rad = curr_azimuth * (float)(M_PI / 180.f);
        elevation_rad = curr_elevation * (float)(M_PI / 180.f);
    }
}

void close_game()
{
    printf("Simulacia ukoncena!\n");

    close_file(data_file);
    printf("Data zapisane do subora!\n");

    glutLeaveMainLoop();
}

char* generate_file_path(char* path)
{
    char base_name[BUFFER_SIZE] = {0};
    char extension[BUFFER_SIZE] = {0};
    char* generated_name = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const char* dot = strrchr(path, '.');

    time_t raw_time;
    struct tm* time_info;
    struct timeval tv;
    time(&raw_time);
    time_info = localtime(&raw_time);
    gettimeofday(&tv, NULL);

    if (dot != NULL)
    {
        strncpy(base_name, path, dot - path);
        base_name[dot - path] = '\0';
        strcpy(extension, dot);
    }
    else
    {
        strcpy(base_name, path);
        strcpy(extension, "");
    }

    snprintf(
            generated_name, BUFFER_SIZE, "%s_%02d-%02d-%04d_%02d-%02d-%02d-%03ld%s",
            base_name, time_info->tm_mday, time_info->tm_mon + 1,
            time_info->tm_year + 1900, time_info->tm_hour,
            time_info->tm_min, time_info->tm_sec, tv.tv_usec/1000, extension
    );

    return generated_name;
}