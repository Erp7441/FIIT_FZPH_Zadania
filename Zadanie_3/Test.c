// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

// GL libraries
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Konštanty
const int casKrok = 10;
const float g = (float) 9.81;

// Vstupne hodnoty
float h, v0, k;

// Suradnice
float y0n = 0;

// Cas
float t = 0, tVyp = 0;

// Rychlost
float vn = 0;

// Subor
FILE * f = NULL;

// Logika
int hore = 1, odraz = 0, odrHore = 0;

void posun(const int hod) {
    tVyp += 10;
    t += 10;

    if (hore) {
        y0n = (float) ((v0 * (t / 1000)) - (0.5 * g * (t / 1000) * (t / 1000)));
        vn = (float) (v0 - (g * (t / 1000)));
        odrHore = 0;
        if (vn <= 0) {
            vn = 0;
            hore = 0;
        } else if (y0n >= h) {
            v0 = vn * k;
            hore = 0;
            t = 0;
            odrHore = 1;
            ++odraz;
        }
    } else {
        if (odrHore) {
            printf("%f\n", t/1000);
            y0n = (float) (h - (v0 * (t / 1000)) - (0.5 * g * (t / 1000) * (t / 1000)));
            vn = (float) (v0 + (g * (t / 1000)));
        } else {
            y0n = (float) ((v0 * (t / 1000)) - (0.5 * g * (t / 1000) * (t / 1000)));
            vn = (float) (v0 - (g * (t / 1000)));
        }

        if (y0n <= 0) {
            if (odrHore == 0) {
                printf("\nPocet odrazeni: %d\n", odraz);
                printf("Rychlost v1 = %.2f", (sqrtf(2 * g * h * (1 - (1 / powf(k, 2)) + (1 / powf(k, 4))))));
                exit(1);
            } else {
                v0 = vn * k;
                hore = 1;
                t = 0;
                odrHore = 0;
            }
        }
    }

    fprintf(f, "%f %f %f\n", (tVyp / 1000), y0n, vn);

    glutPostRedisplay();
    glutTimerFunc(casKrok, posun, hod + 1); // rekurzívna funkcia
}

void resize (int sirka , int vyska ) {
    glViewport (0, 0, sirka, vyska);
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();

    if ( sirka == 0) sirka ++;

    const float pomstr = ((float) vyska )/ ((float)  sirka);

    gluOrtho2D ( 0, 60 * pomstr, 0, 35 * pomstr);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode ( GL_MODELVIEW );

    glLoadIdentity();
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(0, h);
    glVertex2f(60, h);
    glEnd();

    glLoadIdentity();
    // 235, 131, 52
    glColor3f((float) (235.0/256.0), (float) (131.0/256.0), (float) (52.0/256.0));
    glTranslatef((float) 0.0, (float) y0n, (float) 0.0);
    glBegin(GL_QUADS);
    glVertex2f(1, 1);
    glVertex2f(2, 1);
    glVertex2f(2, 0);
    glVertex2f(1, 0);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    h = 2; v0 = 10; k = 1;

    if (f == NULL) {
        f = fopen("vystup.dat", "w");
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(170, 50);
    glutCreateWindow("Uloha cislo 3 - Zaprazny");

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.9, 0.9, 0.9, 0.5);

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc(casKrok, posun, 0);

    glutMainLoop();
    fclose(f);
    return 0;
}