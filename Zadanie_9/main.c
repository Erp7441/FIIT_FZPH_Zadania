
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// TODO:: Graf y/t , v/t , rho/y

#define FPS 60

#define P_0 101325.f
#define R 8.31446261815324f
#define P_N2 powf(0.785f, 43.f)
#define P_O2 (1-powf(0.785f, 43.f))
#define M_N2 0.028f
#define M_O2 0.032f
#define M (P_N2 * M_N2 + P_O2 * M_O2)


void get_data();
void draw();
// Update
void update(int i);
void keyboard_handler(unsigned char key, int x, int y);

float calculate_velocity(float t);
float calculate_position(float t);
float calculate_land_time(float v_start, float y);


// Input parameters
float C = 0.5f;
float S = 10.f;
float v = 100.f;

// Globals
float rho = 1.225f;
float y_0 = 0.9f;
float boost_factor = 1.5f;

// Temperature in kelvin
float T = 273.15f;

// Parachute
float p_y = 0.f;
float p_w = 0.1f;
float t_d = 0.f;

float prev_p_y = 0.f;

float anim_time = 0.f;

bool start = false;

char data_header[] = "y,v,rho,t\n";
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
	if(!start) return;

	prev_p_y = p_y;

	p_y = y_0 - to_meters(calculate_position(anim_time));
	if (p_y < -1.f) p_y = -1.f;

	if (prev_p_y != p_y)
	{
		t_d = calculate_land_time(v, p_y);

		// Update density
		rho = P_0 * (M / (R * T)) * expf(-(M * G) / (R * T) * from_meters(p_y));
	}

	float p_v = calculate_velocity(anim_time);

	printf("y = %.2f, v = %.2f, rho = %.2f, t = %.2f\n", p_y, p_v, rho, anim_time);
	write_to_file(data_file, "%.2f,%.2f,%.2f,%.2f\n", p_y, p_v, rho, anim_time);
}

void update(const int i)
{
    anim_time = display_frame(update_pos);
	if(!start)
	{
		glutTimerFunc(TIME_STEP, update, i + 1);
		return;
	}

	if (p_y > -1.f)
		glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{
	p_y = y_0;  // Toto spravi graficky bug v prvy frame

    data_file = open_file(generate_file_path("../data.csv"), "w");
    write_to_file(data_file, data_header);

    printf("Press Shift+A to start the animation...\n");
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(1.f, 1.f, 0.f);
	draw_quad(0.f, p_y, 0.f, p_w, 0.f);


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

float calculate_velocity(float t)
{
	float v_inf = sqrtf((2 * G * v) / (C * rho * S));
	float tanh_term = tanhf((G * t) / v_inf);
	float result = v_inf * tanh_term;
	return result * boost_factor;
}

float calculate_position(float t)
{
	float integral = 0.0f;
	for (float tau = 0.0f; tau <= t; tau += 1.0f / FPS)
	{
		integral += calculate_velocity(tau) * (1.0f / FPS);
	}
	return y_0 + integral;
}

float calculate_land_time(float v_start, float y)
{
	y = from_meters(y + 1.f);
	float v_inf_start = sqrtf((2 * G * v_start) / (C * rho * S));
	return (v_inf_start / G) * acoshf(expf(y * y / (v_inf_start * v_inf_start)));
}