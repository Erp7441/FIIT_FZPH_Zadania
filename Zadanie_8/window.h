#ifndef ZADANIE_4_FPS_H
#define ZADANIE_4_FPS_H

#endif //ZADANIE_4_FPS_H

#define FPS 60
#define TIME_STEP (1000 / FPS)

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>

#pragma once

unsigned int start_time = 0;
unsigned int last_frame_time = 0;
unsigned int elapsed = 0;  // Cas framu
float time_elapsed = 0.f;  // Celkovy cas


float display_frame(void(*frame_rate_independent_operations)(void))
{
    unsigned int current_time = glutGet(GLUT_ELAPSED_TIME);
    time_elapsed = (float)(current_time - start_time) / 1000.0f;

    elapsed = current_time - last_frame_time;

    if (elapsed > TIME_STEP)
    {
        last_frame_time = current_time - (elapsed % TIME_STEP);
	    frame_rate_independent_operations();
        glutPostRedisplay();
    }

    return time_elapsed;
}

void initialize_window(void(*get_data)(void), void(*draw)(void), void(*update)(int), char* title, int width, int
height, bool fullscreen)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);

    get_data();

    glutCreateWindow(title);
    if (fullscreen) glutFullScreen();
    glutDisplayFunc(draw);

    glutTimerFunc(TIME_STEP, update, 0);
}

void start_animation()
{
    start_time = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}