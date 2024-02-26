// Nakladiak ide rovnomerne
// Auto ide zrychlene


#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// Constants
//#define COORDS
//#define PATH
#define FPS 240
#define TIME_STEP (1000 / FPS)


// Deklaracia funkcii
void update_movement(float frame_delta_time);
void update(int i);
void draw_quads();
void draw_quad(float x, float y, float z);
void get_input_params();

// Cas
unsigned int start_time = 0;
unsigned int last_frame_time = 0;
unsigned int elapsed = 0;  // Cas framu
float time_elapsed = 0.f;  // Celkovy cas


// Vstupne parametre (zadavane pred zacatim animacie)
float acc_x1 = 4.f;
float vel_x2 = 20.f;


// X-ove suradnice kociek
float coord_x1 = 0.f;
float coord_x2 = 0.f;
float path_x1 = 0.f;
float path_x2 = 0.f;


// Pociatocne body
float start_x1 = -2.f;
float start_x2 = -2.f;


// Skalovanie
float scale = 0.01f;  // Skalovanie na metre


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1920, 1080);

    // Ziskanie vstupnych paramterov od usera.
    get_input_params();

    glutCreateWindow("OpenGL: Zadanie 2");
    glutFullScreen();
    start_time = glutGet(GLUT_ELAPSED_TIME);
    glutDisplayFunc(&draw_quads);

    glutTimerFunc(TIME_STEP, update, 0);
    glutMainLoop();

    return 0;
}


void update(const int i)
{
    unsigned int current_time = glutGet(GLUT_ELAPSED_TIME);
    time_elapsed = (float)(current_time - start_time) / 1000.0f;

    elapsed = current_time - last_frame_time;

    if (elapsed > TIME_STEP)
    {
        last_frame_time = current_time - (elapsed % TIME_STEP);
        glutPostRedisplay();
    }

    update_movement(time_elapsed);
    glutTimerFunc(TIME_STEP, update, i + 1);

}

void update_movement(float t)
{

    // Vzorec: x = 1/2 * a * t^2
    float mov_x1 = (0.5f * acc_x1) * (t*t);
    // Vzorec: s = v * t
    float mov_x2 = vel_x2 * t;

    coord_x1 = start_x1 + (mov_x1 * scale);
    coord_x2 = start_x2 + (mov_x2 * scale);

    #ifdef COORDS
        printf("X1 = %.4f\n", coord_x1);
        printf("X2 = %.4f\n", coord_x2);
    #endif

    if (coord_x1 >= coord_x2)
    {
        glutLeaveMainLoop();
        draw_quads();

        // Vzorec: t = 2*v / a
        float cas = (2 * vel_x2) / acc_x1;
        printf("Vypocitany cas: %.3f", cas);

        printf("\nVozidla sa stretli v case = %.3f\n", time_elapsed);

        #ifdef PATH
            path_x1 = fabsf(coord_x1 - start_x1);
            path_x2 = fabsf(coord_x2 - start_x2);
            printf("Cesta auta: %.3f\n", path_x1);
            printf("Cesta nakladiaka: %.3f\n", path_x2);
        #endif
    }
}


void draw_quad(float x, float y, float z)
{
    glLoadIdentity();

    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);

    glEnd();
}
void draw_quads()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);  // Modra
    draw_quad(coord_x1, -0.35f, 0.f);

    glColor3f(0.0f, 1.0f, 0.0f);  // Zelena
    draw_quad(coord_x2, 0.35f, 0.f);

    glutSwapBuffers();
}


void get_input_params()
{
    printf("Zadajte zrychlenie  auta (modre): ");
    scanf("%f", &acc_x1);
    getchar();

    printf("Zadajte rychlost nakladiaka (zelene): ");
    scanf("%f", &vel_x2);
    getchar();
}