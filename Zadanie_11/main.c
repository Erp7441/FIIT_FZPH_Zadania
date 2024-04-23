
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
void draw_balls();
// Update
void update(int i);
// Keyboard
void key_handler(unsigned char key, int x, int y);
// Utility
void reset_simulation(bool hard);
void close_simulation();


// Simulation
float anim_time = 0.f;
float reset_time = 0.f;

typedef struct {
	float x, y;
} Vector;

typedef struct {
	float r, g, b;
} Color;

typedef struct {
	Vector min, max;
} Bounds;

typedef struct {
	const int id;
	Vector pos, prev_pos, vel;
	const float r, m;
	Color c;
} Ball;


Ball balls[] = {
{
		.id = 1,
		.pos = { 0.f, 0.f },
		.prev_pos = { 0.f, 0.f },
		.vel = { 0.5f, 1.f },
		.r = 0.03f,
		.m = 1.f,
		.c = { 1.f, 0.f, 0.f }
	},
{
		.id = 2,
		.pos = { 0.5f, 0.5f },
		.prev_pos = { 0.f, 0.f },
		.vel = { 0.f, 0.f },
		.r = 0.03f,
		.m = 1.f,
		.c = { 0.f, 0.f, 1.f }
	},
};


Bounds bounds = {
	.min = { -1.f, -1.f },
	.max = { 1.f, 1.f }
};

// File
char data_header[] = "x,y,z,time\n";
FILE* data_file = NULL;  // CSV file containing data



int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 11", 1920, 1080, true, true, GLUT_RGB | GLUT_DOUBLE);

    glutKeyboardFunc(key_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void ball_collided_bounds(Ball* b)
{
	if (b->pos.x < bounds.min.x || b->pos.x > bounds.max.x) {
		b->vel.x = -b->vel.x;
	}

	if (b->pos.y < bounds.min.y || b->pos.y > bounds.max.y) {
		b->vel.y = -b->vel.y;
	}
}

void ball_move(Ball* b) {
	printf("Ball %d - pos: %.3f, %.3f\n", b->id, b->pos.x, b->pos.y);
	b->prev_pos.x = b->pos.x;
	b->prev_pos.y = b->pos.y;
	b->pos.x += b->vel.x;
	b->pos.y += b->vel.y;

	ball_collided_bounds(b);
}

void update_balls(void(*func)(Ball*))
{
	int balls_count = sizeof(balls) / sizeof(Ball);
	for (int i = 0; i < balls_count; i++) {
		func(&(balls[i]));
	}
}


void update_pos() {
	update_balls(ball_move);
}




void update(const int i)
{
    anim_time = display_frame(update_pos) - reset_time;

    glutTimerFunc(TIME_STEP, update, i + 1);
}


void get_data()
{
    data_file = open_file(generate_file_path("../data.csv"), "w"); // FILE
    write_to_file(data_file, data_header); // FILE


    printf("Press any key to start...\n");
    getchar();

	printf("Starting animation...\n");
}

void draw_balls()
{
	int balls_count = sizeof(balls) / sizeof(Ball);
	for (int i = 0; i < balls_count; i++) {
		glColor3f(balls[i].c.r, balls[i].c.g, balls[i].c.b);
		draw_circle(balls[i].pos.x, balls[i].pos.y, 0.f, balls[i].r*2.f);

	}
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_balls();
    glutSwapBuffers();
}


void key_handler(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':

            break;
        case 'r':
	        reset_simulation(false);
            break;
	    case 'R':
		    reset_simulation(true);
		    break;
        case 'G':
            get_data();
		    reset_simulation(false);
            break;
        case 27: // Esc
	        close_simulation();
            break;
        default:
            break;
    }
}



// Utility
void reset_simulation(bool hard)
{
    write_to_file(data_file, data_header); // FILE


	reset_time += anim_time ;
    anim_time = 0.0f;
}



void close_simulation()
{
    printf("Simulacia ukoncena!\n");

    close_file(data_file); // FILE
    printf("Data zapisane do subora!\n");

    glutLeaveMainLoop();
}
