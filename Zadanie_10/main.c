
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define BUFFER_SIZE 256

void get_data();
void draw();
// Update
void update(int i);
// Keyboard
void key_handler(unsigned char key, int x, int y);
void special_key_handler(int key, int x, int y);
// Mouse
void mouse_wheel_handler(int wheel, int direction, int x, int y);
// Utility
void reset_simulation();
void reset_camera();
void close_simulation();


// Simulation
float anim_time = 0.f;
bool bounce = false;

const float UK = 0.1f;
const float THETA = M_PI / 4;

// Bullet
typedef struct {
	float azimuth;  // 45 stupnov
	float elevation; // 45 stupnov
	float x; float y; float z;
	float vx; float vy; float vz;
	float ax; float ay; float az;
	float m;
	float force;
	const float s_x; const float s_y; const float s_z;
	const float size;
	float azimuth_rad; float elevation_rad;
	float curr_dist; float curr_azimuth; float curr_elevation;
} Ball;

// Window bounds
typedef struct {
	const float max_x; const float max_y; const float max_z;
	const float min_x; const float min_y; const float min_z;
} Bounds;

// Camera
typedef struct {
	float angle_horizontal; float angle_vertical;
	float angle_horizontal_s; float angle_vertical_s;
	float speed; float distance; float distance_s;
	float x; float y; float z;
	float center_x; float center_y; float center_z;
	float center_s_x; float center_s_y; float center_s_z;
	float up_x; float up_y; float up_z;
} Camera;


Ball ball = {
	.azimuth = 5.f, .elevation = 0.f, .x = 0.f, .y=0.f, .z=0.f,
	.vx= 0.02f, .vy= 0.03f, .vz= 0.01f,
	.ax= 0.0f, .ay= 0.0f, .az= 0.0f,
	.m = 1.0f, .force = 0.0f,
	.s_x= 0.f, .s_y= 0.f, .s_z= 0.f,
	.size= 0.05f,
	.azimuth_rad= 0.f, .elevation_rad = 0.f, .curr_dist= 0.f, .curr_azimuth = 0.f, .curr_elevation = 0.f
};

Bounds bounds = {
	.max_x = 2.51f, .max_y = 1.f, .max_z = 2.51f,
	.min_x = 0.f, .min_y = 0.f, .min_z = 0.f
};

Camera cam = {
	.angle_horizontal = 3.9f, .angle_horizontal_s = 3.9f,
	.angle_vertical = 1.4f, .angle_vertical_s = 1.4f,
	.speed = 0.1f, .distance = 2.85f, .distance_s = 2.85f,
	.x = 0.0f, .y = 0.0f, .z = 0.0f,
	.center_x = 0.0f, .center_y = 0.0f, .center_z = 0.0f,
	.center_s_x = 0.0f, .center_s_y = 0.0f, .center_s_z = 0.0f,
	.up_x = 0.0f, .up_y = 1.0f, .up_z = 0.0f
};


// File
char data_header[] = "x,y,z,time\n";
FILE* data_file = NULL;  // CSV file containing data



int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 10", 1920, 1080, true, true, GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutKeyboardFunc(key_handler);
    glutSpecialFunc(special_key_handler);
    glutMouseWheelFunc(mouse_wheel_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}


void update_pos()
{
	if (!bounce) return;

	ball.x += ball.vx * anim_time + 0.5f * ball.ax * (anim_time * anim_time);
	ball.y += ball.vy * anim_time + 0.5f * ball.ay * (anim_time * anim_time);
	ball.z += ball.vz * anim_time + 0.5f * ball.az * (anim_time * anim_time);

	ball.vx += ball.ax * anim_time;
	ball.vy += ball.ay * anim_time;
	ball.vz += ball.az * anim_time;

	// Kontrola prekročenia hraníc kocky
	if (ball.x + ball.size >= bounds.max_x || ball.x - ball.size <= bounds.min_x) {
		ball.vx = -ball.vx; // Zmena smeru na opačný
	}
	if (ball.y + ball.size >= bounds.max_y || ball.y - ball.size <= bounds.min_y) {
		ball.vy = -ball.vy;
	}
	if (ball.z + ball.size >= bounds.max_z || ball.z - ball.size <= bounds.min_z) {
		ball.vz = -ball.vz;
	}

	// Vypočítanie nových rýchlostí po zrážke
	float delta_px = 2 * ball.m * ball.vx * cosf(ball.azimuth_rad);
	float delta_py = 2 * ball.m * ball.vy * cosf(ball.elevation_rad);
	float delta_pz = 2 * ball.m * ball.vz * cosf(ball.azimuth_rad);

	// Vypočítaj nové rýchlosti
	ball.vx = delta_px / anim_time;
	ball.vy = delta_py / anim_time;
	ball.vz = delta_pz / anim_time;

	printf ("X: %.3f, Y: %.3f, Z: %.3f\n", ball.x, ball.y, ball.z);
}

void update(const int i)
{
    anim_time = display_frame(update_pos);

    write_to_file(data_file, "%f,%f,%f,%f\n", ball.x, ball.y, anim_time); // FILE
    glutTimerFunc(TIME_STEP, update, i + 1);
}


void get_data()
{
    data_file = open_file(generate_file_path("../data.csv"), "w"); // FILE
    write_to_file(data_file, data_header); // FILE


    printf("Press any key to start...\n");
    getchar();
}


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 10.0);

    cam.x = sinf(cam.angle_vertical) * cosf(cam.angle_horizontal) * cam.distance;
    cam.y = cosf(cam.angle_vertical) * cam.distance;
    cam.z = sinf(cam.angle_vertical) * sinf(cam.angle_horizontal) * cam.distance;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        cam.x, cam.y, cam.z,
        cam.center_x, cam.center_y, cam.center_z,
        cam.up_x, cam.up_y, cam.up_z
    );

    draw_axes_3d(100.0f, 0.f, 0.f, 0.f);

    glColor3f(0.3f, 0.5f, 0.1f);
    draw_cube(2.51f, 0.f, 2.51f, 5.f, 0.f, 5.f);
    draw_cube_edges(2.51f, 1.f, 2.51f, 2.51f, 1.f, 2.51f);

    draw_ball_fade(ball.x, ball.y, ball.z, ball.size, 1.f, 0.f, 0.f);

    glutSwapBuffers();
}


void key_handler(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
			bounce = true;
            break;
        case 'r':
	        reset_simulation();
            break;
        case 'G':
            get_data();
		    reset_simulation();
            break;
        case 'C':
            reset_camera();
            break;
        case 27: // Esc
	        close_simulation();
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
            if (modifiers == GLUT_ACTIVE_SHIFT) cam.center_x -= cam.speed;
            else cam.angle_horizontal -= cam.speed;
            break;
        case GLUT_KEY_LEFT:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam.center_x += cam.speed;
            else cam.angle_horizontal += cam.speed;
            break;
        case GLUT_KEY_UP:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam.center_z -= cam.speed;
            else cam.angle_vertical += cam.speed;
            break;
        case GLUT_KEY_DOWN:
            if (modifiers == GLUT_ACTIVE_SHIFT) cam.center_z += cam.speed;
            else cam.angle_vertical -= cam.speed;
            break;
        default:
            break;
    }
}

void mouse_wheel_handler(int wheel, int direction, int x, int y)
{
    if (direction > 0)
	    cam.distance *= 1.1f;
    else
	    cam.distance /= 1.1f;

    glutPostRedisplay();
}


// Utility
void reset_simulation()
{
//    write_to_file(data_file, data_header); FILE
	bounce = false;
	ball.x = ball.s_x;
	ball.y = ball.s_y;
	ball.z = ball.s_z;

    reset_camera();

    anim_time = 0.f;
    ball.curr_dist = 0.f;
    ball.curr_azimuth = 0.f;
    ball.curr_elevation = 0.f;
}

void reset_camera()
{
    cam.center_x = cam.center_s_x;
    cam.center_y = cam.center_s_y;
    cam.center_z = cam.center_s_z;
    cam.distance = cam.distance_s;
    cam.angle_horizontal = cam.angle_horizontal_s;
    cam.angle_vertical = cam.angle_vertical_s;
}


void close_simulation()
{
    printf("Simulacia ukoncena!\n");

    close_file(data_file); // FILE
    printf("Data zapisane do subora!\n");

    glutLeaveMainLoop();
}
