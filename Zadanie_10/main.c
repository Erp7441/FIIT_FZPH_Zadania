
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
void reset_simulation(bool hard);
void reset_camera();
void close_simulation();
void check_bounds(float* value, float offset, float max_bound, float min_bound);
void check_max_m(float* m_x, float* m_y, float* m_z, float max_val);


// Simulation
float anim_time = 0.f;
float reset_time = 0.f;
bool bounce = false;


// Bullet
typedef struct {
	float x; float y; float z;
	float vx; float vy; float vz;
	float ax; float ay; float az;
	float m;
	float force;
	const float s_x; const float s_y; const float s_z;
	const float size;
} Ball;

// Window box
typedef struct {
	const float max_x; const float max_y; const float max_z;
	const float min_x; const float min_y; const float min_z;
} Box;

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
	.x = 0.f, .y=0.f, .z=0.f,
	.vx= 1.0f, .vy= 1.5f, .vz= -1.5f,
	.ax= 0.0f, .ay= 0.0f, .az= 0.0f,
	.m = 1.0f, .force = 0.0f,
	.s_x= 0.f, .s_y= 0.f, .s_z= 0.f,
	.size= 0.05f,
};

Box box = {
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

float delta_p(float v, float m, float alfa)
{
	return 2 * m * v * cosf(alfa);
}


float calc_alpha(Ball o, float normal_x, float normal_y, float normal_z) {
	// Vypočítanie dotykového produktu
	float dot_product = o.vx * normal_x + o.vy * normal_y + o.vz * normal_z;

	// Vypočítanie dĺžok vektorov
	float v_length = sqrtf(o.vx * o.vx + o.vy * o.vy + o.vz * o.vz);
	float n_length = sqrtf(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);

	// Vypočítanie kosínusu uhla nárazu pomocou dotykového produktu a dĺžok vektorov
	float cos_alpha = dot_product / (v_length * n_length);

	// Vrátenie uhla nárazu v radiánoch
	return acosf(cos_alpha);
}


void update_pos() {
	if (!bounce) return;

	if (anim_time < 10.f) anim_time = 10.f;

	// Kontrola kolízie so stenami
	if (ball.x + ball.size >= box.max_x || ball.x - ball.size <= box.min_x)
	{
		// Zmena smeru
		ball.vx = -ball.vx;


		// Výpočet normálového vektoru steny
		float normal_x = (ball.x + ball.size >= box.max_x) ? -1.0f : 1.0f;

		// Zmena hybnosti
		float delta_p_x = delta_p(ball.vx, ball.m, calc_alpha(ball, normal_x, 0.0f, 0.0f));
		// Aplikácia zmeny hybnosti na rýchlosť
		ball.vx += delta_p_x / anim_time;
		printf("Delta p_x: %.3f\n", delta_p_x);
	}
	if (ball.y + ball.size >= box.max_y || ball.y - ball.size <= box.min_y)
	{
		// Zmena smeru
		ball.vy = -ball.vy;
		// Zmena hybnosti
		float delta_p_y = delta_p(ball.vy, ball.m, calc_alpha(ball, 0.0f, 1.0f, 0.0f));
		// Aplikácia zmeny hybnosti na rýchlosť
		ball.vy += delta_p_y / anim_time;
		printf("Delta p_y: %.3f\n", delta_p_y);
	}
	if (ball.z + ball.size >= box.max_z || ball.z - ball.size <= box.min_z)
	{
		// Zmena smeru
		ball.vz = -ball.vz;
		// Zmena hybnosti
		float delta_p_z = delta_p(ball.vz, ball.m, calc_alpha(ball, 0.0f, 0.0f, 1.0f));
		// Aplikácia zmeny hybnosti na rýchlosť
		ball.vz += delta_p_z / anim_time;
		printf("Delta p_z: %.3f\n", delta_p_z);
	}

	// Aktualizácia pozície
	ball.x += to_meters(ball.vx * anim_time);
	ball.y += to_meters(ball.vy * anim_time);
	ball.z += to_meters(ball.vz * anim_time);

	// Aktualizácia rýchlosti
	ball.vx += ball.ax * anim_time;
	ball.vy += ball.ay * anim_time;
	ball.vz += ball.az * anim_time;

	check_bounds(&ball.x, ball.size, box.max_x, box.min_x);
	check_bounds(&ball.y, ball.size, box.max_y,  box.min_y+ball.size);
	check_bounds(&ball.z, ball.size, box.max_z, box.min_z);
	check_max_m(&ball.vx, &ball.vy, &ball.vz, 1.f);
	check_max_m(&ball.ax, &ball.ay, &ball.az, 1.f);

	printf("X: %.3f, Y: %.3f, Z: %.3f\n", ball.x, ball.y, ball.z);
}




void update(const int i)
{
    anim_time = display_frame(update_pos) - reset_time;

    write_to_file(data_file, "%f,%f,%f,%f\n", ball.x, ball.y, anim_time); // FILE
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
	        reset_simulation(false);
            break;
	    case 'R':
		    reset_simulation(true);
		    break;
        case 'G':
            get_data();
		    reset_simulation(false);
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
void reset_simulation(bool hard)
{
    write_to_file(data_file, data_header); // FILE
	bounce = false;
	ball.x = ball.s_x;
	ball.y = ball.s_y;
	ball.z = ball.s_z;

	ball.vx= 0.02f;
	ball.vy= 0.03f;
	ball.vz= 0.01f;
	ball.ax= 0.0f;
	ball.ay= 0.0f;
	ball.az= 0.0f;
	ball.force = 0.0f;

	if (hard)
        reset_camera();

	reset_time += anim_time ;
    anim_time = 0.0f;
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

void check_bounds(float* value, float offset, float max_bound, float min_bound)
{
	if (*value+offset >= max_bound + offset) *value = max_bound;
	else if (*value+offset <= min_bound + offset) *value = min_bound;
}

void check_max_m(float* m_x, float* m_y, float* m_z, float max_val)
{
	if (*m_x > max_val) *m_x = max_val;
	else if (*m_x < -max_val) *m_x = -max_val;
	if (*m_y > max_val) *m_y = max_val;
	else if (*m_y < -max_val) *m_y = -max_val;
	if (*m_z > max_val) *m_z = max_val;
	else if (*m_z < -max_val) *m_z = -max_val;
}