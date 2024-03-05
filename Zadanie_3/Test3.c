#include <GL/freeglut.h>
#include <math.h>

// Konstanty
const double g = 9.81; // Gravitační zrychlení (m/s^2)

// Globální proměnné
double v0; // Počáteční rychlost (m/s)
double h;  // Počáteční výška (m)
double t_d; // Čas trvání pádu (s)

// Funkce pro výpočet času trvání pádu
void compute_fall_time() {
    // Kvadratická rovnice pro čas trvání pádu
    double a = -0.5 * g;
    double b = v0;
    double c = -h;
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        t_d = (-b + sqrt(discriminant)) / (2 * a);
    } else {
        // Pokud je diskriminant záporný, těleso nikdy nevyletí do vzduchu
        t_d = 0.0;
    }
}

// Funkce pro vykreslení míčku
void draw_ball() {
    glColor3f(1.0, 0.0, 0.0); // Červená barva
    glutSolidSphere(0.1, 20, 20); // Velikost míčku
}

// Funkce pro vykreslení scény
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Vykreslení míčku
    glPushMatrix();
    glTranslated(0.0, h - 0.1, 0.0); // Posun míčku na správnou výšku
    draw_ball();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Inicializace GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hadání míčku do vzduchu");

    // Nastavení funkcí
    glutDisplayFunc(display);

    // Zadejte počáteční rychlost a výšku
    v0 = 20.0; // m/s
    h = 10.0; // m
    compute_fall_time();

    // Spuštění GLUT loop
    glutMainLoop();

    return 0;
}
