
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
//#define VERBOSE
//#define BALL_TIME_COLLISION_TEST
//#define DEBUG_LINES

#ifdef BALL_TIME_COLLISION_TEST
	#define INIT_N 2
#else
	#define INIT_N 1
#endif

void get_data();
void draw();
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
bool running = false;

Ball* balls = NULL;
int N = INIT_N;

Bounds bounds = {
	.min = { -1.77f, -0.99999633f },
	.max = { 1.77f, 0.99999633f }
};

// File
char data_header[] = "id,pos_x,pos_y,vel_x,vel_y,time,crash_ball,crash_border\n";
FILE* data_file = NULL;  // CSV file containing data


int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

    glutInit(&argc, argv);

    // Initializes GLUT window
    initialize_window(get_data, draw, update, "OpenGL: Zadanie 11", 1920, 1080, true, false, GLUT_RGB | GLUT_DOUBLE);

    glutKeyboardFunc(key_handler);

    // Reshapes objects based on window size
    glutReshapeFunc(resize);

    // Starts GLUT main loop
    start_animation();

	if (data_file != NULL) close_file(data_file); // FILE

	return 0;
}

void write_ball_to_file(Ball* ball)
{
	if (ball == NULL) return;
	write_to_file(data_file, "%d,%f,%f,%f,%f,%f,%f,%f\n", ball->id, ball->pos.x, ball->pos.y, ball->vel.x, ball->vel.y,
				  anim_time, ball->t.crash_ball, ball->t.crash_border);
}

void update_pos() {
	if (!running) return;
	update_balls_2(check_ball_collision_and_apply_impulse, balls, N, true);
	update_balls(check_wall_collision_and_apply_impulse, balls, N);
	update_balls(ball_move, balls, N);
	update_balls_2(calculate_ball_collision_time, balls, N, true);
	update_balls(calculate_wall_collision_time, balls, N);
	update_balls(write_ball_to_file, balls, N);
	print_balls(balls, N);
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

	if (get_confirmation("Do you want to use input data? (Y/n):"))
	{
		#ifdef VERBOSE
			params._m_set = input("Enter ball weight (0.0 ... 1.0):\n", &params.m, "%f");
		#else
			params._m_set = input("Enter ball weight (1.f):\n", &params.m, "%f");
		#endif

		if (!params._m_set || params.m < 0.0f || params.m > 1.0f)
		{
			#ifdef VERBOSE
				printf("Using default value.\n");
			#endif
			params._m_set = false;
		}

		params._pos_set = input_vector("Enter ball position vector (x y):\n", &params.pos);
		#ifdef VERBOSE
			if (!params._pos_set) printf("Using random values.\n");
		#endif

		params._vel_set = input_vector("Enter ball velocity vector (x y):\n", &params.vel);
		#ifdef VERBOSE
			if (!params._vel_set) printf("Using random values.\n");
		#endif

		params._r_set = input("Enter ball radius (0.03f):\n", &params.r, "%f");
		#ifdef VERBOSE
			if (!params._r_set) printf("Using default value.\n");
		#endif
	}
	else
	{
		printf("Using default parameters:\n");
		printf("Ball weight: %.3f\n", params.m);
		printf("Ball position: random\n");
		printf("Ball velocity: random\n");
		printf("Ball radius: %.3f\n\n", params.r);
	}

	balls = generate_balls(N);

	#ifdef BALL_TIME_COLLISION_TEST
		N = 2;
		// Test1
		balls[0].pos.x = 0.f; balls[0].pos.y = 0.f;
		balls[0].vel.x = -0.4f; balls[0].vel.y = 0.0f;

		balls[1].pos.x = -1.f; balls[1].pos.y = 0.f;
		balls[1].vel.x = 0.4f; balls[1].vel.y = 0.0f;

		// Test 2
//		balls[0].pos.x = -0.35f; balls[0].pos.y = 0.f;
//		balls[0].vel.x = -0.1f; balls[0].vel.y = 0.1f;
//
//		balls[1].pos.x = -0.75f; balls[1].pos.y = 0.25f;
////		balls[1].vel.x = 0.4f; balls[1].vel.y = -0.4f;
//		balls[1].vel.x = 0.1f; balls[1].vel.y = -0.1f;

	#endif

	printf("Press any key to start...\n");
    getchar();
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

#ifdef DEBUG_LINES
	// Draw line where ball[0] is headed.
	for (int i = 0; i < N; i++) {
		glColor3f(1.f, 0.f, 0.f);
		draw_trace_line(balls[i]);
	}
#endif

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
		case ' ':
			running = true;
			break;
		case 'A':
			balls = add_balls(balls, &N, N+1);
			break;
		case 'D':
			remove_balls(balls, &N, N-1);
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

	running = false;
	balls = generate_balls(N);

	reset_time += anim_time ;
    anim_time = 0.0f;
}


void close_simulation()
{
    printf("Simulacia ukoncena!\n");

    close_file(data_file); // FILE
	data_file = NULL;
    printf("Data zapisane do subora!\n");

    glutLeaveMainLoop();
}



