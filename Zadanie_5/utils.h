#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>

#ifndef ZADANIE_4_UTILS_H
#define ZADANIE_4_UTILS_H
#endif //ZADANIE_4_UTILS_H

#define METERS_COEF 100.f
#define G 9.81f

#define DEBUG

#pragma once

void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (w <= h)
    {
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    }
    else
    {
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float get_meters(char* prompt)
{
    float value = 0.f;
    printf(prompt);
    scanf("%f", &value);
    getchar();
    return value / (METERS_COEF);
}

float to_meters(float value)
{
    return value / METERS_COEF;
}

float from_meters(float value)
{
    return value * METERS_COEF;
}

void printf_debug(const char* format, ...)
{
    #ifdef DEBUG
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    #endif
}