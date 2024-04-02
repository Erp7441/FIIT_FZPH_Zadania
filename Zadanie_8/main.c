
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>


#define FPS 60


void get_data();
void draw();
// Update
void update(int i);
void keyboard_handler(unsigned char key, int x, int y);


// Input parameters
float slope_angle = 30.f;
float engine_force = 10.f;
float friction = 0.3f;
float max_velocity = 8.f;

// Globals
float slope_length = 100.f;
float slope_angle_rad = 0.f;

float quad_x = 0.f;
float quad_y = 0.f;
float quad_w = 0.05f;
float quad_mass = 0.5f;
float quad_vel = 0.f;
float acc = 0.f;
float x2 = 0.f;
float prev_quad_x = -1.f;
float prev_quad_y = -1.f;

float motor_turn_off_line_x = 0.f;
float motor_turn_off_line_y = 0.f;
float motor_turn_off_line_length = 100.f;

float anim_time = 0.f;

bool start = false;
bool max_vel_reached = false;

char data_header[] = "quad_x,quad_y,time,max_vel_reached\n";
FILE* data_file = NULL;  // CSV file containing data

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 8", 1920, 1080, true);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Keyboard handler
    glutKeyboardFunc(keyboard_handler);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void update_pos()
{
	x2 = to_meters(0.5f * acc * (anim_time * anim_time));
	if (max_vel_reached) x2 += to_meters(quad_mass*quad_vel) * anim_time;

	quad_x += x2 * cosf(slope_angle_rad);
	quad_y += x2 * sinf(slope_angle_rad);

	if(quad_x < 0.f) quad_x = 0.f;
	if(quad_y < 0.f) quad_y = 0.f;
}

void update(const int i)
{
    anim_time = display_frame(update_pos);
    if(!start)
    {
        glutTimerFunc(TIME_STEP, update, i + 1);
        return;
    }

    float quad_g_force = quad_mass * G;
    float force_t = friction * quad_g_force * cosf(slope_angle_rad);  // F_t
    float force_n = quad_g_force * sinf(slope_angle_rad);  // F_g
    float force_net = engine_force - force_n - force_t;  // F
    acc = force_net / quad_mass;

    if (!max_vel_reached) quad_vel = acc * anim_time;  // Derivacia vzorca x' (resp. x2 v kode)
    if (quad_vel > max_velocity && engine_force != 0)
    {
        motor_turn_off_line_x = quad_x;
        motor_turn_off_line_y = quad_y;
        engine_force = 0.f;
        max_vel_reached = true;
        printf("Max velocity reached!\n");
    }

    if (quad_x != prev_quad_x || quad_y != prev_quad_y)
    {
        write_to_file(data_file, "%f,%f,%f,%s\n", quad_x, quad_y, anim_time, max_vel_reached ? "true" : "false");
        prev_quad_x = quad_x;
        prev_quad_y = quad_y;
    }

    glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{
	if (!get_confirmation("Use defaults? (Y/n)"))
	{
		if (!input("Enter slope angle (30):", &slope_angle, "%f"))
			slope_angle = 30.f;
		if (!input("Enter engine force (10):", &engine_force, "%f"))
			engine_force = 10.f;
		if (!input("Enter friction coeficient (0.3):", &friction, "%f"))
			friction = 0.3f;
		if (!input("Enter max velocity (8):", &max_velocity, "%f"))
			max_velocity = 8.f;
	}
	printf("Slope angle: %.3f\n", slope_angle);
	printf("Engine force: %.3f\n", engine_force);
	printf("Friction coeficient: %.3f\n", friction);
	printf("Max velocity: %.3f\n", max_velocity);

    slope_angle_rad = slope_angle * (float)M_PI / 180.f;

    data_file = open_file(generate_file_path("../data.csv"), "w");
    write_to_file(data_file, data_header);

    printf("Press Shift+A to start the animation...\n");
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);


    if (max_vel_reached)
    {
        glColor3f(1.f, 0.f, 0.f);
        draw_slope_line(
                motor_turn_off_line_x,
                motor_turn_off_line_y,
                motor_turn_off_line_length,
                slope_angle + 90
            );
        glColor3f(1.f, 0.f, 0.f);
    }
    else glColor3f(0.f, 1.f, 0.f);
    draw_quad(quad_x, quad_y, 0.f, quad_w, slope_angle);

    glColor3f(1.f, 1.f, 0.f);
    draw_slope_line(0, 0, slope_length, slope_angle);


    glutSwapBuffers();
}


void keyboard_handler(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'A':
            if (start) break;
            printf("Animation started!\n");
            start = true;
            start_time = glutGet(GLUT_ELAPSED_TIME);
            break;
        case 27:
            printf("Exiting...\n");
            close_file(data_file);
            exit(0);
        default:
            break;
    }
}