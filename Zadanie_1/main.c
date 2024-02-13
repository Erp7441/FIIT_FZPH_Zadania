#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>

int start_time = 0;


// Vstupne parametre
float vel_x1 = 1.0f;
float vel_x2 = 1.0f;
float path_len = 0.f;


const int time_step = 25; // v milisekundach
float coord_x1 = 0.f;
float coord_x2 = 0.f;
float path_x1 = 0.f;
float path_x2 = 0.f;
float time = 1.f / 60.f;


// parametre su S - vzdialenost medzi vozidlami (draha)
// v1 - rychlost
// v2 - rychlost
// vystupom bude vziadlenost ktoru preslo prve a druhe vozidlo
// a ci zadana draha je = vzialenosti prveho a druheho vozidla


void update(const int i)
{
    // s = v * t
    coord_x1 -= vel_x1 * time;
    coord_x2 += vel_x2 * time;

    // Udrziavanie dlzky prejdenej drahy
    path_x1 += vel_x1 * time;
    path_x2 += vel_x2 * time;

//    if (path_x1 >= path_len) {  // Keby chces aby obe presli celu drahu
    if (coord_x1 < coord_x2 + 0.5f) {
        int end_time = glutGet(GLUT_ELAPSED_TIME);
        float elapsed = (float)(end_time - start_time) / 1000.0f;


        glutLeaveMainLoop();
        printf("\nPrejdena draha: \nX1 = %f\nX2 = %f", path_x1, path_x2);

        float path_len_calc = path_x1 + path_x2;
        printf("\nS1 + S2 = %f", path_len_calc);
        printf("\nZadana draha = %f", path_len);
        printf("\nDEBUG: Elapsed - %fs", elapsed);
    }

    glutPostRedisplay();
    glutTimerFunc(time_step, update, i + 1);
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


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1080, 640);
    glutInitWindowPosition(200, 150);

    printf("Zadajte dlzku drahy S: ");
    scanf("%f", &path_len);
    getchar();

    printf("Zadajte rychlost kocky v1 (Modra): ");
    scanf("%f", &vel_x1);
    getchar();

    printf("Zadajte rychlost kocky v2 (Zelena): ");
    scanf("%f", &vel_x2);
    getchar();

    coord_x1 = (path_len + 0.5f) / 2.f;  // Treba ratat s tym ze draha by sa nam predlzila o nasu sirku. To nechceme.
    coord_x2 = (-path_len - 0.5f) / 2.f;

    start_time = glutGet(GLUT_ELAPSED_TIME);

    glutCreateWindow("OpenGL: Zadanie 1");
    glutDisplayFunc(&draw_quads);

    glutTimerFunc(time_step, update, 0);
    glutMainLoop();
    return 0;
}
