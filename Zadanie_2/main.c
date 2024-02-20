
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// Nakladiak ide rovnomerne
// Auto ide zrychlene


// Deklaracia funkcii
void update_movement(float frame_delta_time);
void update(int i);

void draw_quads();
void draw_quad(float x, float y, float z);

void get_input_params();

float time_elapsed = 0.f;

// Vstupne parametre (zadavane pred zacatim animacie)
float vel_x1 = 1.0f;
float vel_x2 = 1.0f;
float acc_x1 = 1.f;
float acc_x2 = 1.f;


// FPSka
const int frame_rate = 60;
const unsigned int time_step = 1000 / frame_rate;


// X-ove suradnice kociek
float coord_x1 = -.5f;
float coord_x2 = -.5f;

// Celkovy cas
int frame_last_time = 0;

// Boolean pre stret
int b_meet = 0;


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1920, 1080);

    // Ziskanie vstupnych paramterov od usera.
    get_input_params();

    glutCreateWindow("OpenGL: Zadanie 1");
    glutFullScreen();
    glutDisplayFunc(&draw_quads);

    glutTimerFunc(time_step, update, 0);
    glutMainLoop();

    return 0;
}


void update(const int i)
{
    if (b_meet)
    {

        glutLeaveMainLoop();
        draw_quads();


        // Vzorec: t = 2*v / a
        float cas = (2 * vel_x1) / (acc_x1);
        printf("X1 cas: %.4f", cas);

        float cas2 = (2 * vel_x2) / (acc_x2);
        printf("X2 cas: %.4f\n", cas2);



    }

    int current_time = glutGet(GLUT_ELAPSED_TIME);

    // Cas kolko trvalo vygenerovanie framu
    float frame_delta_time = 0.f;
    if (frame_last_time != 0)
    {
        // Aktualny cas - cas konca posledneho framu = cas generovania aktualneho framu
        frame_delta_time = (float)(current_time - frame_last_time) / 1000.0f;
        time_elapsed += frame_delta_time;
    }

    // Vykresli novy frame
    update_movement(frame_delta_time);

    glutPostRedisplay();
    glutTimerFunc(time_step, update, i + 1);

    // Cas konca framu
    frame_last_time = current_time;
}

void update_movement(float frame_delta_time)
{
    if (b_meet) return;

    float mov_x1 = acc_x1 * (frame_delta_time*frame_delta_time);
    float mov_x2 = vel_x2 * frame_delta_time;

    coord_x1 += mov_x1;
    coord_x2 += mov_x2;

    printf("X1 = %.4f\n", coord_x1);
    printf("X2 = %.4f\n", coord_x2);

//    if (coord_x1 == coord_x2) b_meet = 1;
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

    printf("Zadajte rychlost  auta (modre): ");
    scanf("%f", &vel_x1);
    getchar();

    printf("Zadajte zrychlenie  auta (modre): ");
    scanf("%f", &acc_x1);
    getchar();

    printf("Zadajte rychlost nakladiaka (zelene): ");
    scanf("%f", &vel_x2);
    getchar();
}