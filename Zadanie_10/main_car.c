
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

typedef enum type {
	BULLET = 0,
	CAR = 1
} Type;

typedef struct object {
	Type type;
	float m,v,x,y,w,h;
	float initial_x, initial_y;
} Object;

void get_data();
void draw();
// Update
void update(int i);
void keyboard_handler(unsigned char key, int x, int y);

float calculate_velocity(float t);
float calculate_position(float t);
float calculate_land_time(float v_start, float y);


// Input parameters


// Globals
Object small_car = {CAR, 0.1f, 100.f, -0.9f, 0.f, 0.05f, 0.05f};
Object big_car = {CAR, 0.5f, 90.f, -0.5f, 0.f, 0.3f, 0.1f};

float x_0 = 0.f;

float anim_time = 0.f;

bool start = false;
bool collided = false;


//char data_header[] = "y,v,rho,t\n";
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

	small_car.v = (small_car.v *  (big_car.m - small_car.m) - 2 * small_car.m * big_car.v) / (small_car.m + big_car.m);
	big_car.v = (2 * small_car.m * small_car.v + big_car.v * (big_car.m - small_car.m)) / (small_car.m + big_car.m);


	small_car.x = small_car.initial_x + to_meters(small_car.v * anim_time);
	big_car.x = big_car.initial_x + to_meters(big_car.v * anim_time);





	printf("sc_x: %.4f, sc_v: %.4f, bc_x: %.4f, bc_v: %.4f, col: %d, x_0: %.4f\n", small_car.x, small_car.v, big_car.x,
		   big_car.v,
	       collided, x_0);
}

void update(const int i)
{
    anim_time = display_frame(update_pos);
	if(!start)
	{
		glutTimerFunc(TIME_STEP, update, i + 1);
		return;
	}

	glutTimerFunc(TIME_STEP, update, i + 1);
}

void get_data()
{

	x_0 = (small_car.x) - (big_car.x);

	small_car.v = to_meters(1000.f);
	big_car.v = to_meters(900.f);

	small_car.initial_x = small_car.x;
	small_car.initial_y = small_car.y;
	big_car.initial_x = big_car.x;
	big_car.initial_y = big_car.y;

    data_file = open_file(generate_file_path("../data.csv"), "w");
//    write_to_file(data_file, data_header);

    printf("Press Shift+A to start the animation...\n");
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);


	// Bullet
    glColor3f(1.f, 1.f, 0.f);
	draw_quad(small_car.x, small_car.y, small_car.w, small_car.h, 0.f);

	// Car
	glColor3f(0.f, 1.f, 0.f);
	draw_quad(big_car.x, big_car.y, big_car.w, big_car.h, 0.f);


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
