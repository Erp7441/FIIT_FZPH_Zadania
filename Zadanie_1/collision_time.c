// parametre su S - vzdialenost medzi vozidlami (draha)
// v1 - rychlost
// v2 - rychlost
// vystupom bude vziadlenost ktoru preslo prve a druhe vozidlo
// a ci zadana draha je = vzialenosti prveho a druheho vozidla

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define DEBUG_STOP  // Stopne program po kolizii
//#define DEBUG  // Zapne debugovacie vypisi


// Pociatocny cas animacie
int start_time = 0;


// Vstupne parametre (zadavane pred zacatim animacie)
float vel_x1 = 1.0f;
float vel_x2 = 1.0f;
float path_len = 0.f;


// FPSka
const int frame_rate = 240;  // Optimal value je 240 FPS ale zalezi hlavne od rychlosti kociek
const unsigned int time_step = 1000 / frame_rate;


// X-ove suradnice kociek
float coord_x1 = 0.f;
float coord_x2 = 0.f;


// Dlzka drah kociek
float path_x1 = 0.f;
float path_x2 = 0.f;


// Celkovy cas
int frame_last_time = 0;
float collision_time = 0.f;
float elapsed_time = 0.f;


// Deklaracia funkcii
void update_movement(float frame_delta_time);
void check_collision();
void update(int i);
void draw_quads();
void draw_quad(float x, float y, float z);


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1080, 640);
    glutInitWindowPosition(200, 150);

    printf("Zadajte dlzku drahy S v metroch: ");
    scanf("%f", &path_len);
    getchar();

    if (path_len < 0) {
        printf("Dlzka drahy nemoze byt nulova");
        return 1;
    }

    printf("Zadajte rychlost kocky v1 (Modra): ");
    scanf("%f", &vel_x1);
    getchar();

    printf("Zadajte rychlost kocky v2 (Zelena): ");
    scanf("%f", &vel_x2);
    getchar();

    // Vypocet casu kolizie pomocou rovnomerneho priamociareho pohybu: t = s / (v_1 + v_2)
    collision_time = path_len / (vel_x1 + vel_x2);

    // Treba ratat s tym ze draha by sa nam predlzila o nasu sirku. To nechceme.
    coord_x1 = (path_len + 0.5f) / 2.f;
    coord_x2 = (-path_len - 0.5f) / 2.f;


    start_time = glutGet(GLUT_ELAPSED_TIME);  // Cas zacatia animacie

    glutCreateWindow("OpenGL: Zadanie 1");  // Vytvorenie okna
    glutDisplayFunc(&draw_quads);  // Vykreslenie stvorcov

    glutTimerFunc(time_step, update, 0);  // Timer na update funkciu
    glutMainLoop();  // Main loop vykreslovania
    return 0;
}


void update(const int i)
{
    int current_time = glutGet(GLUT_ELAPSED_TIME);

    // Cas kolko trvalo vygenerovanie framu
    float frame_delta_time = 0.f;
    if (frame_last_time != 0)
    {
        // Aktualny cas - cas konca posledneho framu = cas generovania aktualneho framu
        frame_delta_time = (float)(current_time - frame_last_time) / 1000.0f;
    }

    #ifdef DEBUG
        printf("Collision elapsed time: %f \t Frame delta time: %f\n", elapsed_time, frame_delta_time);
    #endif

    update_movement(frame_delta_time);
    check_collision();

    // Kolko casu nam ubehlo do teraz
    elapsed_time = (float)(current_time - start_time) / 1000.0f;
    elapsed_time -= frame_delta_time;

    // Vykresli novy frame
    glutPostRedisplay();
    glutTimerFunc(time_step, update, i + 1);

    // Cas konca framu
    frame_last_time = glutGet(GLUT_ELAPSED_TIME);
}


void check_collision()
{
    if (elapsed_time >= collision_time)
    {
        #ifdef DEBUG
            int end_time = glutGet(GLUT_ELAPSED_TIME);
            float animation_elapsed_time = (float)(end_time - start_time) / 1000.0f;
        #endif

        glutPostRedisplay();

        #ifdef DEBUG_STOP
            printf("Press any key to continue...");
            getchar();
        #endif

        glutLeaveMainLoop();
        printf("\nPrejdena draha: \nX1 = %f\nX2 = %f", path_x1, path_x2);

        //! Vypocet drahy
        float path_len_calc = path_x1 + path_x2;
        printf("\nS1 + S2 = %f", path_len_calc);

        printf("\nZadana draha = %f", path_len);

        #ifdef DEBUG
            printf("\nDEBUG: Animation elapsed - %fs", animation_elapsed_time);
            printf("\nDEBUG: Collision time - %fs", collision_time);
            printf("\nDEBUG: Collision elapsed - %fs", elapsed_time);

            float over_shoot = path_len_calc - path_len;
            printf("\n\n-------------------------------------");
            printf("\nOvershoot bol: %f", over_shoot);
            path_x1 -= over_shoot / 2;
            path_x2 -= over_shoot / 2;
        #endif

    }
}


void update_movement(float frame_delta_time)
{
    // s = v * t
    float mov_x1 = vel_x1 * frame_delta_time;
    float mov_x2 = vel_x2 * frame_delta_time;

    coord_x1 -= mov_x1;
    coord_x2 += mov_x2;

    // Udrziavanie dlzky prejdenej drahy
    path_x1 += mov_x1;
    path_x2 += mov_x2;
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
