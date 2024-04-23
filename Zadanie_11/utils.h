#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifndef ZADANIE_4_UTILS_H
#define ZADANIE_4_UTILS_H
#endif //ZADANIE_4_UTILS_H

#define METERS_COEF 100.f
#define G 9.80665f

#define BUFFER_SIZE 256

#define DEBUG

#pragma once

typedef struct {
	float x, y;
} Vector;

typedef struct {
	float r, g, b;
} Color;

typedef struct {
	Vector min, max;
} Bounds;

typedef enum {
	X = 0, Y = 1, Z = 2
} Axis;

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

void flush_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

bool get_confirmation(char* prompt)
{
    char answer = '\0';
	int count;
	do
	{
		printf(prompt);
		count = scanf("%c", &answer);

		if (answer == '\n' && count == 1) return true;
		else flush_buffer();
	} while((answer != 'y' && answer != 'n' && answer != 'Y' && answer != 'N') || count != 1);

    return answer == 'y' || answer == 'Y';
}

bool input(char* prompt, void* address, char* modifier)
{
	printf(prompt);
	char str[BUFFER_SIZE+3];
	fgets(str, BUFFER_SIZE, stdin);
	if (strncmp(str, "\n", 1) == 0) return false;
	return sscanf(str,modifier, address) == 1;
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

void check_bounds(float* value, float offset, float max_bound, float min_bound)
{
	if (*value+offset >= max_bound + offset) *value = max_bound;
	else if (*value+offset <= min_bound + offset) *value = min_bound;
}

float random_number(float max, float min)
{
	float range = max - min;
	float random = ((float) rand()) / (float) RAND_MAX;
	return (random * range) + min;
}