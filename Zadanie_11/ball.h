#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

#define MAX_BALLS 15

#pragma once

Bounds bounds;

typedef struct {
	float crash_border;
	float crash_ball;
} Time;

typedef struct {
	int id;
	Vector pos, prev_pos, vel;
	float r, m;
	Color c;
	Time t;
} Ball;


bool balls_collided_ref(Ball* b1, Ball* b2, float* dx, float* dy, float* dist);
bool balls_collided(Ball* b1, Ball* b2);
bool ball_collided_bounds(Ball* b, Axis a);
void calculate_ball_collision_time(Ball* b1, Ball* b2);
void calculate_wall_collision_time(Ball* b);
Ball generate_ball(int id);
Ball* generate_balls(int count);
void ball_move(Ball* b);
void update_balls(void(*func)(Ball*), Ball* balls, int count);
void print_ball(Ball ball);
void print_balls(Ball* balls, int count);
Ball* add_balls(Ball* balls, int *original_count, int new_count);
Ball* remove_balls(Ball* balls, int *original_count, int new_count);

void ball_move(Ball* b) {
	b->prev_pos.x = b->pos.x;
	b->prev_pos.y = b->pos.y;
	b->pos.x += to_meters(b->vel.x);
	b->pos.y += to_meters(b->vel.y);

}

void check_wall_collision_and_apply_impulse(Ball* b)
{
	if (ball_collided_bounds(b, X))
		b->vel.x = -b->vel.x;
	if (ball_collided_bounds(b, Y))
		b->vel.y = -b->vel.y;
}

void check_ball_collision_and_apply_impulse(Ball* b1, Ball* b2)
{
	if (b1->id == b2->id) return;

	float dx; // r_1
	float dy; // r_2
	float dist; // r_12
	if (balls_collided_ref(b1, b2, &dx, &dy, &dist))
	{
		Vector r_12 = {dx, dy};
		Vector p_12 = {b2->vel.x - b1->vel.x, b2->vel.y - b1->vel.y};
		float r_12_squared = r_12.x * r_12.x + r_12.y * r_12.y;

		float dot = r_12.x * p_12.x + r_12.y * p_12.y; // Sucin vektorov z tabule (I_x sekcia)
		Vector imp = {dot * r_12.x / r_12_squared, dot * r_12.y / r_12_squared};

		b1->vel.x += imp.x / b1->m;
		b1->vel.y += imp.y / b1->m;

		b2->vel.x -= imp.x / b2->m;
		b2->vel.y -= imp.y / b2->m;

	}
}

void update_balls(void(*func)(Ball*), Ball* balls, int count)
{
	for (int i = 0; i < count; i++)
	{
		func(&(balls[i]));
	}
}

void update_balls_2(void(*func)(Ball*, Ball*), Ball* balls, int count, bool skip_same_ele)
{
	for (int i = 0; i < count; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			if (skip_same_ele && i == j) continue;
			func(&(balls[i]), &(balls[j]));
		}
	}
}

// Utilities
void print_ball(Ball ball)
{
	printf(
		"Ball %d: pos = { %f, %f }, vel = { %f, %f }, c = { %f, %f, %f }, t = { %f, %f }\n",
		ball.id,
		ball.pos.x, ball.pos.y,
		ball.vel.x, ball.vel.y,
		ball.c.r, ball.c.g, ball.c.b,
		ball.t.crash_border, ball.t.crash_ball
	);
}

void print_balls(Ball* balls, int count)
{
	if (balls == NULL) return;
	for (int i = 0; i < count; i++)
		print_ball(balls[i]);
}

Ball generate_ball(int id)
{
	float offset = 0.03f * 2.f;

	return (Ball){
		.id = id,
		.pos = {
			random_number(bounds.max.x - offset, bounds.min.x + offset),
			random_number(bounds.max.y - offset, bounds.min.y + offset)
		},
		.prev_pos = { 0.f, 0.f },
		.vel = { random_number(1.f, 0.f), random_number(1.f, 0.f) },
		.r = offset / 2.f,
		.m = 1.f,
		.c = {
			random_number(1.f, 0.1f),
			random_number(1.f, 0.1f),
			random_number(1.f, 0.1f)
		}
	};
}

bool ball_pos_taken(Ball* ball, Ball* balls, int i)
{
	for (int j = 0; j <= i; j++) {
		if (balls_collided(ball, &(balls[j])))
			return true;
	}
	return false;
}

Ball* generate_balls(int count)
{
	Ball* balls = (Ball*) calloc(count, sizeof(Ball));
	for (int i = 0; i < count; i++) {

		Ball* ball = (Ball*) malloc(sizeof(Ball));
		*ball = generate_ball(i);

		if (ball_pos_taken(ball, balls, i)) {
			free(ball);
			i--;
			continue;
		}

		balls[i] = *ball;
		print_ball(balls[i]);
	}

	return balls;
}

Ball* add_balls(Ball* balls, int *original_count, int new_count)
{
	if (*original_count > MAX_BALLS || new_count > MAX_BALLS) return balls;
	int original = *original_count;

	Ball* new_balls = (Ball*) calloc( (original + new_count), sizeof(Ball));
	if (new_balls == NULL)
	{
		printf("Error: alloc failed\n");
		return balls;
	}

	for (int i = 0; i < original; i++)
		new_balls[i] = balls[i];

	for (int i = original; i < new_count; i++)
	{
		Ball* ball = (Ball*) malloc(sizeof(Ball));
		*ball = generate_ball(i);

		if (ball_pos_taken(ball, balls, i)) {
			free(ball);
			i--;
			continue;
		}

		new_balls[i] = *ball;
		print_ball(new_balls[i]);
	}

	*original_count = new_count;
	return new_balls;
}

Ball* remove_balls(Ball* balls, int *original_count, int new_count)
{
	// TODO:: Implement?
}

void calculate_ball_collision_time(Ball* b1, Ball* b2) {
	// Koeficienty kvadratickej rovnice
	float r12x = b2->pos.x - b1->pos.x;
	float r12y = b2->pos.y - b1->pos.y;
	float v12x = b2->vel.x - b1->vel.x;
	float v12y = b2->vel.y - b1->vel.y;
	float R = b1->r * 2.0f; // Predpokladáme rovnaký polomer pre obe guľky

	float a = v12x * v12x + v12y * v12y;
	float b = 2.0f * (r12x * v12x + r12y * v12y);
	float c = r12x * r12x + r12y * r12y - R * R;

	// Diskriminánt kvadratickej rovnice
	float discriminant = b * b - 4.0f * a * c;

	// Ak je diskriminánt kladný, existujú dva reálne korene
	if (discriminant >= 0) {
		float t1 = (-b + sqrtf(discriminant)) / (2.0f * a);
		float t2 = (-b - sqrtf(discriminant)) / (2.0f * a);

		// Vráť menší z koreňov
		b1->t.crash_ball = fminf(t1, t2);
		b2->t.crash_ball = fminf(t1, t2);
	} else {
		// V opačnom prípade neexistuje reálny koreň, takže neexistuje kolízia
		b1->t.crash_ball = INFINITY;
		b2->t.crash_ball = INFINITY;
	}

	// TODO:: fix permanent infinity
}


void calculate_wall_collision_time(Ball* b) {
	float t_x = INFINITY;
	float t_y = INFINITY;

	// Vypočítaj čas kolízie s hranicami v oboch osiach (x a y)
	if (b->vel.x > 0) {
		t_x = (bounds.max.x - (b->pos.x + b->r)) / b->vel.x;
	} else if (b->vel.x < 0) {
		t_x = ((b->pos.x - b->r) - bounds.min.x) / (-b->vel.x);
	}

	if (b->vel.y > 0) {
		t_y = (bounds.max.y - (b->pos.y + b->r)) / b->vel.y;
	} else if (b->vel.y < 0) {
		t_y = ((b->pos.y - b->r) - bounds.min.y) / (-b->vel.y);
	}

	// Vyber menší z časov kolízie v oboch osiach
	b->t.crash_border = fminf(t_x, t_y);
}

bool balls_collided(Ball* b1, Ball* b2)
{
	float d1 = b1->r * 2.f;
	float d2 = b2->r * 2.f;
	float dx = b1->pos.x - b2->pos.x;
	float dy = b1->pos.y - b2->pos.y;
	float dist = sqrtf(dx * dx + dy * dy);
	return dist < d1 + d2;  // TODO:: Fix stucking into each other bug
}

bool balls_collided_ref(Ball* b1, Ball* b2, float* dx, float* dy, float* dist)
{
	float d1 = b1->r * 2.f;
	float d2 = b2->r * 2.f;
	*dx = b1->pos.x - b2->pos.x;
	*dy = b1->pos.y - b2->pos.y;
	*dist = sqrtf(*dx * *dx + *dy * *dy);
	if (*dx * *dx + *dy * *dy < 0) printf("was?\n");
	return *dist < d1 + d2;
}

bool ball_collided_bounds(Ball* b, Axis a)
{
	float offset = b->r * 2.f + 0.01f;
	if (a == X && b->pos.x - offset <= bounds.min.x || b->pos.x + offset >= bounds.max.x)
		return true;

	if (a == Y && b->pos.y - offset  <= bounds.min.y || b->pos.y + offset >= bounds.max.y)
		return true;

	return false;
}