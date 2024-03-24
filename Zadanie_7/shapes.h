//
// Created by Martin on 05/03/2024.
//

#ifndef ZADANIE_7_SHAPES_H
#define ZADANIE_7_SHAPES_H

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

void draw_cube(float x, float y, float z, float sx, float sy, float sz)
{
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glBegin(GL_QUADS);

        // Front face
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        // Back face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);

        // Top face
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);

        // Bottom face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);

        // Right face
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);

        // Left face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();

    glPopMatrix();
}

void draw_cube_edges(float x, float y, float z, float sx, float sy, float sz)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    glBegin(GL_LINES);
    // bottom square
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);

    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);

    glVertex3f(1, 1, -1);
    glVertex3f(-1, 1, -1);

    glVertex3f(-1, 1, -1);
    glVertex3f(-1, -1, -1);

    // top square
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);

    glVertex3f(1, -1, 1);
    glVertex3f(1, 1, 1);

    glVertex3f(1, 1, 1);
    glVertex3f(-1, 1, 1);

    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, 1);

    // vertical lines
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);

    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1);

    glVertex3f(1, 1, -1);
    glVertex3f(1, 1, 1);

    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glEnd();

    glPopMatrix();
}

void draw_ball(float x, float y, float z, float radius)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    const int NUM_RAYS = 360;
    const int NUM_LAYERS = 180;
    const float PI_2 = 2*(float)M_PI;

    for(int j = 0; j < NUM_LAYERS; j++)
    {
        float theta1 = (float)j * PI_2 / (float)NUM_LAYERS;
        float theta2 = (float)(j + 1) * PI_2 / (float)NUM_LAYERS;

        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i <= NUM_RAYS; i++)
        {
            float theta3 = (float)i * PI_2 / (float)NUM_RAYS;

            float ex = cosf(theta2) * cosf(theta3);
            float ey = sinf(theta2);
            float ez = cosf(theta2) * sinf(theta3);
            float px = x + radius * ex;
            float py = y + radius * ey;
            float pz = z + radius * ez;

            glVertex3f(px, py, pz);

            ex = cosf(theta1) * cosf(theta3);
            ey = sinf(theta1);
            ez = cosf(theta1) * sinf(theta3);
            px = x + radius * ex;
            py = y + radius * ey;
            pz = z + radius * ez;

            glVertex3f(px, py, pz);
        }
        glEnd();
    }

    glPopMatrix();
}

void draw_ball_fade(float x, float y, float z, float radius, float r, float g, float b)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    const int NUM_RAYS = 360;
    const int NUM_LAYERS = 180;
    const float PI_2 = 2*(float)M_PI;

    for(int j = 0; j < NUM_LAYERS; j++)
    {
        float theta1 = (float)j * PI_2 / (float)NUM_LAYERS;
        float theta2 = (float)(j + 1) * PI_2 / (float)NUM_LAYERS;

        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i <= NUM_RAYS; i++)
        {
            float theta3 = (float)i * PI_2 / (float)NUM_RAYS;

            float ex = cosf(theta2) * cosf(theta3);
            float ey = sinf(theta2);
            float ez = cosf(theta2) * sinf(theta3);
            float px = x + radius * ex;
            float py = y + radius * ey;
            float pz = z + radius * ez;

            glColor3f(r * fabsf(sinf(theta3)), g * fabsf(cosf(theta3)), b * fabsf(sinf(theta2)));
            glVertex3f(px, py, pz);

            ex = cosf(theta1) * cosf(theta3);
            ey = sinf(theta1);
            ez = cosf(theta1) * sinf(theta3);
            px = x + radius * ex;
            py = y + radius * ey;
            pz = z + radius * ez;

            glColor3f(r * fabsf(sinf(theta3)), g * fabsf(cosf(theta3)), b * fabsf(sinf(theta2)));
            glVertex3f(px, py, pz);
        }
        glEnd();
    }

    glPopMatrix();
}

void draw_axes(float length, float x0, float y0, float z0) {
    glBegin(GL_LINES);

    // X red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0 + length, y0, z0);

    // Y green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0 + length, z0);

    // Z blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0, z0 + length);

    glEnd();
}

