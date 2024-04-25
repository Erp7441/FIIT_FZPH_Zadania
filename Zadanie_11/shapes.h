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
#include "ball.h"

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

void draw_axes_3d(float length, float x0, float y0, float z0) {
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

void draw_bounds(float max_x, float max_y, float min_x, float min_y) {
	glLoadIdentity();

	glBegin(GL_LINES);
	glVertex2d(min_x, min_y);
	glVertex2d(max_x, min_y);
	glVertex2d(max_x, max_y);
	glVertex2d(min_x, max_y);

	glVertex2d(min_x, min_y);
	glVertex2d(min_x, max_y);
	glVertex2d(max_x, min_y);
	glVertex2d(max_x, max_y);
	glEnd();
}

void draw_trace_line(Ball ball)
{
	float angle = atan2f(ball.vel.y, ball.vel.x);
	float length = sqrtf(ball.vel.x * ball.vel.x + ball.vel.y * ball.vel.y);
	float x1 = ball.pos.x + cosf(angle) * length;
	float y1 = ball.pos.y + sinf(angle) * length;
	float x2 = ball.pos.x - cosf(angle) * length;
	float y2 = ball.pos.y - sinf(angle) * length;

	glLoadIdentity();
	glLineWidth(30.0f);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1.f);
}