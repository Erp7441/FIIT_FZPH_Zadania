// parametre su S - vzdialenost medzi vozidlami (draha)
// v1 - rychlost
// v2 - rychlost
// vystupom bude vziadlenost ktoru preslo prve a druhe vozidlo
// a ci zadana draha je = vzialenosti prveho a druheho vozidla

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>


// Deklaracia funkcii
void update_movement(float frame_delta_time);
void update(int i);

void draw_quads();
void draw_quad(float x, float y, float z);

void get_input_params();


// Vstupne parametre (zadavane pred zacatim animacie)
float vel_x1 = 1.0f;
float vel_x2 = 1.0f;
float path_len = 0.f;


// FPSka
const int frame_rate = 60;
const unsigned int time_step = 1000 / frame_rate;


// X-ove suradnice kociek
float coord_x1 = 0.f;
float coord_x2 = 0.f;


// Dlzka drah kociek
float path_x1 = 0.f;
float path_x2 = 0.f;

// Celkovy cas
int frame_last_time = 0;

// Boolean pre kolizu
int b_collided = 0;

// Kolko cesty nam ostava este prejst
float remaning_path = 0.f;


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1920, 1080);

    // Ziskanie vstupnych paramterov od usera.
    get_input_params();
    remaning_path = path_len;

    // Inicializacia pozicii
    coord_x1 = (path_len + 0.5f) / 2.f;  // Treba ratat s tym ze draha by sa nam predlzila o nasu sirku. To nechceme.
    coord_x2 = (-path_len - 0.5f) / 2.f;

    glutCreateWindow("OpenGL: Zadanie 1");
    glutDisplayFunc(&draw_quads);

    glutTimerFunc(time_step, update, 0);
    glutMainLoop();

    return 0;
}


void update(const int i)
{
    // Narazili sme na predchadzajucom frame?
    if (b_collided)
    {
        glutLeaveMainLoop();

        //! Vypocet drahy
        float path_len_calc = path_x1 + path_x2;

        printf("\nPrejdena draha: \nX1 (modre vozidlo) = %f\nX2 (zelene vozidlo) = %f", path_x1, path_x2);
        printf("\nS1 + S2 = %f", path_len_calc);
        printf("\nZadana draha = %f", path_len);
    }

    int current_time = glutGet(GLUT_ELAPSED_TIME);

    // Cas kolko trvalo vygenerovanie framu
    float frame_delta_time = 0.f;
    if (frame_last_time != 0)
    {
        // Aktualny cas - cas konca posledneho framu = cas generovania aktualneho framu
        frame_delta_time = (float)(current_time - frame_last_time) / 1000.0f;
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
    if (b_collided) return;

    // s = v * t
    float mov_x1 = vel_x1 * frame_delta_time;
    float mov_x2 = vel_x2 * frame_delta_time;


    if (remaning_path - mov_x1 < 0.f)
    {
        mov_x1 = remaning_path;
        b_collided = 1;
    }
    coord_x1 -= mov_x1;
    path_x1 += mov_x1;
    remaning_path -= mov_x1;


    if (remaning_path - mov_x2 < 0.f)
    {
        mov_x2 = remaning_path;
        b_collided = 1;
    }
    coord_x2 += mov_x2;
    path_x2 += mov_x2;
    remaning_path -= mov_x2;
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

    glColor3f(0.0f, 0.0f, 1.0f);
    draw_quad(coord_x1, 0.f, 0.f);

    glColor3f(0.0f, 1.0f, 0.0f);
    draw_quad(coord_x2, 0.f, 0.f);

    glutSwapBuffers();
}


void get_input_params()
{
    // Ziskanie vstupnych parametrov
    printf("Zadajte dlzku drahy S: ");
    scanf("%f", &path_len);
    getchar();
    if (path_len < 0) {
        printf("Dlzka drahy nemoze byt nulova");
        exit(-1);
    }

    printf("Zadajte rychlost kocky v1 (modre vozidlo): ");
    scanf("%f", &vel_x1);
    getchar();

    printf("Zadajte rychlost kocky v2 (zelene vozidlo): ");
    scanf("%f", &vel_x2);
    getchar();
}