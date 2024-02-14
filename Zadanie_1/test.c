#include <GL/glut.h>
#include <stdio.h>

// Mouse coordinates
int mouseX = 0;
int mouseY = 0;

// World coordinates
double worldX = 0.0;
double worldY = 0.0;
double worldZ = 0.0;

// Display function (not used in this example)
void display() {
    // Your rendering code goes here (if needed)
}

// Mouse move callback
void mouseMove(int x, int y) {
    mouseX = x;
    mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y; // Flip Y-coordinate

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Get the world coordinates
    gluUnProject(mouseX, mouseY, 0.0, modelview, projection, viewport, &worldX, &worldY, &worldZ);

    printf("Mouse coordinates (screen): (%d, %d)\n", mouseX, mouseY);
    printf("World coordinates: (%.2f, %.2f, %.2f)\n", worldX, worldY, worldZ);

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600); // Set your desired window size
    glutCreateWindow("Mouse World Coordinates Example");

    // Register callbacks
    glutDisplayFunc(display); // Not used in this example
    glutPassiveMotionFunc(mouseMove); // Track mouse movement

    // Initialize OpenGL settings (if needed)
    // ...

    glutMainLoop();
    return 0;
}
