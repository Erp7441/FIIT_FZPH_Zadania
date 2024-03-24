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


void draw_quad_3d(float x, float y, float z)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-4.f, 4.f, -4.f, 4.f, -4.f, 4.f);
    gluPerspective(0, 1.0, 1.0, 10.0);

    // Front - Red
//    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    // Back - Green
//    gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);

    // Top - Blue
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    // Bottom - Yellow

    // Right - Cyan


    // Left - White
//    gluLookAt(1, 0, 0, 0, 0, 0, 0, 1, 0);


//    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    //Front face
    glColor3f(1.0f, 0.0f, 0.0f);  // Purple
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);

    //Back face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(0.0f, 0.0f, -0.5f);
    glVertex3f(0.5f, 0.0f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.0f, 0.5f, -0.5f);

    //Top face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.0f, 0.5f, -0.5f);

    //Bottom face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, -0.5f);
    glVertex3f(0.0f, 0.0f, -0.5f);

    //Right face
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.0f);

    //Left face
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -0.5f);
    glVertex3f(0.0f, 0.5f, -0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);

    glEnd();
}

