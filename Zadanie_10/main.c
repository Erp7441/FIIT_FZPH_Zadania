
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
Object bullet = {BULLET, 0.1f, 100.f, -0.9f, 0.f, 0.05f, 0.05f};
Object car = {CAR, 0.5f, 0.f, -0.5f, 0.f, 0.3f, 0.1f};

float x_0 = 0.f;

float anim_time = 0.f;

bool start = false;
bool collided = false;
float collision_time = 0.f;


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

	bullet.x = bullet.initial_x + to_meters(bullet.v * anim_time);

	if (!collided)
	{
		collided = bullet.x < car.x + car.w && bullet.x + bullet.w > car.x && bullet.y < car.y + car.h && bullet.y + bullet.h > car.y;
		collision_time = anim_time;
	}

	if (collided)
	{
		car.v = (bullet.m / car.m + bullet.m / car.m) * bullet.v;
		car.x = car.initial_x + to_meters(car.v * (anim_time - collision_time));
	}

	if (bullet.x > car.x) bullet.x = car.x;


	printf("b_x: %.4f, b_v: %.4f, c_x: %.4f, c_v: %.4f, col: %d, x_0: %.4f\n", bullet.x, bullet.v, car.x, car.v,
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

	x_0 = (bullet.x) - (car.x);

	bullet.v = to_meters(1000.f);

	bullet.initial_x = bullet.x;
	bullet.initial_y = bullet.y;
	car.initial_x = car.x;
	car.initial_y = car.y;

    data_file = open_file(generate_file_path("../data.csv"), "w");
//    write_to_file(data_file, data_header);

    printf("Press Shift+A to start the animation...\n");
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);


	// Bullet
    glColor3f(1.f, 1.f, 0.f);
	draw_quad(bullet.x, bullet.y, bullet.w, bullet.h, 0.f);

	// Car
	glColor3f(0.f, 1.f, 0.f);
	draw_quad(car.x, car.y, car.w, car.h, 0.f);


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
