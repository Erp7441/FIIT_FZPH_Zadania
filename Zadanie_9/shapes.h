//
// Created by Martin on 05/03/2024.
//

#ifndef ZADANIE_4_SHAPES_H
#define ZADANIE_4_SHAPES_H

#endif //ZADANIE_4_SHAPES_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>

#pragma once

void draw_circle(float x, float y, float z, float s)
{
    glLoadIdentity();
    glTranslatef(x, y, z);
    glBegin(GL_TRIANGLE_FAN);
    const int NUM_RAYS = 360;
    const double PI_2 = 2*M_PI;

    for(int i = 0; i < NUM_RAYS; i++)
    {
        float cos_x = (float)cos(PI_2 * i/NUM_RAYS);
        float sin_y = (float)sin(PI_2 * i/NUM_RAYS);
        glVertex2f(cos_x * s, sin_y * s);
    }
    glEnd();
}

void draw_quad(float x, float y, float z, float w, float r)
{
    glLoadIdentity();

    glTranslatef(x, y, z);
    glRotatef(r, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(w, 0.0f, 0.0f);
    glVertex3f(w, w, 0.0f);
    glVertex3f(0.0f, w, 0.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);

    glEnd();
}

void draw_axes()
{
    glLoadIdentity();
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
}

void draw_slope_line(float x, float y, float length, float angle)
{
    float radianAngle = angle * M_PI / 180.0f;  // Convert angle to radians
    float x2 = x + length * cos(radianAngle);
    float y2 = y + length * sin(radianAngle);

    glLoadIdentity();
    glBegin(GL_LINES);
    glVertex2f(x, y);  // Starting point of the line
    glVertex2f(x2, y2);  // Ending point of the line
    glEnd();

    return;

    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
}
