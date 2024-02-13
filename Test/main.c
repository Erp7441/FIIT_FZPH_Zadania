#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>

const int icaskrok = 25; // v milisekundach

float surX1 = 1.f;
float surX2 = -1.f;
float rychX1 = 1.0f;
float rychX2 = 1.0f;
float cestX1 = 0.f;
float cestX2 = 0.f;
float cas = 1.f/60.f;

// parametre su S - vzdialenost medzi vozidlami (draha)
// v1 - rychlost
// v2 - rychlost
// vystupom bude vziadlenost ktoru preslo prve a druhe vozidlo
// a ci zadana draha je = vzialenosti prveho a druheho vozidla

void aktualizuj(const int ihod)
{
    // s = v * t
    surX1 -= rychX1 * cas;
    surX2 += rychX2 * cas;

    cestX1 += surX1;
    cestX2 += surX2;

//    printf("X1:%f\nX2:%f\n", surX1, surX2);

    if (surX1 < surX2 + 0.5)
    {
        glutLeaveMainLoop();
        printf("Prejdena draha X1: %f\tX2: %f\n", cestX1, cestX2);
        printf("s = %f", (cestX1+cestX2));
    }

    glutPostRedisplay();
    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}

void vykresliStvorec(float x, float y, float z){
    glLoadIdentity();

    glTranslatef(x, y, z);

    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
}

void kresliStvorce()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    vykresliStvorec(surX1, 0.f, 0.f);


    glColor3f(0.0, 1.0, 0.0);
    vykresliStvorec(surX2, 0.f, 0.f);

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1080, 640);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("OpenGL: Zadanie 1");
    glutDisplayFunc(kresliStvorce);

    glutTimerFunc(icaskrok, aktualizuj, 0);
    glutMainLoop();
    return 0;
}
