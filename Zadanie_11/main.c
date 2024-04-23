
#include "window.h"
#include "utils.h"
#include "shapes.h"
#include "file.h"
#include "ball.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define BUFFER_SIZE 256

void get_data();
void draw();
// Update
void update(int i);
// Keyboard
void key_handler(unsigned char key, int x, int y);
// Utility
void reset_simulation(bool hard);
void close_simulation();

const int INIT_N = 1;

// Simulation
float anim_time = 0.f;
float reset_time = 0.f;

Ball* balls = NULL;
int N = INIT_N;

Bounds bounds = {
	.min = { -1.77f, -0.99999633f },
	.max = { 1.77f, 0.99999633f }
};

// File
char data_header[] = "id,pos_x,pos_y,vel_x,vel_y,time\n";
FILE* data_file = NULL;  // CSV file containing data



int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	balls = generate_balls(N);
    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 11", 1920, 1080, true, false, GLUT_RGB | GLUT_DOUBLE);

    glutKeyboardFunc(key_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

    return 0;
}

void write_ball_to_file(Ball* ball)
{
	if (ball == NULL) return;
	write_to_file(data_file, "%d,%f,%f,%f,%f,%f\n", ball->id, ball->pos.x, ball->pos.y, ball->vel.x, ball->vel.y, anim_time);
}

void update_pos() {
	update_balls(ball_move, balls, N);
	update_balls(write_ball_to_file, balls, N);
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


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.f, 1.f, 0.f);
	draw_bounds(bounds.max.x, bounds.max.y, bounds.min.x, bounds.min.y);

	for (int i = 0; i < N; i++) {
		glColor3f(balls[i].c.r, balls[i].c.g, balls[i].c.b);
		draw_circle(balls[i].pos.x, balls[i].pos.y, 0.f, balls[i].r*2.f);
	}

    glutSwapBuffers();
}


void key_handler(unsigned char key, int x, int y) {
	printf("X: %f, Y: %f\n", balls[1].pos.x, balls[1].pos.y);
    switch (key) {
//		case 'w':
//			balls[1].pos.y += 0.01f;
//			break;
//        case 'd':
//			balls[1].pos.x += 0.01f;
//            break;
//		case 's':
//			balls[1].pos.y -= 0.01f;
//			break;
//		case 'a':
//			balls[1].pos.x -= 0.01f;
//			break;
		case 'A':
			balls = add_balls(balls, &N, N+1);
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

	if (hard)
		N = INIT_N;

	balls = generate_balls(N);

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



