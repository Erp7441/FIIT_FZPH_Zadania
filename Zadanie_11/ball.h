#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

#pragma once

Bounds bounds;

typedef struct {
	int id;
	Vector pos, prev_pos, vel;
	float r, m;
	Color c;
} Ball;

void ball_collided(Ball* b1, Ball* b2)
{
	float d1 = b1->r * 2.f;
	float d2 = b2->r * 2.f;
	float dx = b1->pos.x - b2->pos.x - d1 - d2;
	float dy = b1->pos.y - b2->pos.y - d1 - d2;
	float dist = sqrtf(dx * dx + dy * dy);

	if (dist < b1->r + b2->r) {
		float angle = atan2f(dy, dx);
		float force = (b1->m * b2->m) / (dist * dist);
		b1->vel.x -= to_meters(force * cosf(angle));
		b1->vel.y -= to_meters(force * sinf(angle));
		b2->vel.x += to_meters(force * cosf(angle));
		b2->vel.y += to_meters(force * sinf(angle));
	}
}

void ball_collided_bounds(Ball* b)
{
	float offset = b->r * 2.f;
	if (b->pos.x - offset <= bounds.min.x || b->pos.x + offset >= bounds.max.x)
		b->vel.x = -b->vel.x;

	if (b->pos.y - offset  <= bounds.min.y || b->pos.y + offset >= bounds.max.y)
		b->vel.y = -b->vel.y;
}

void ball_move(Ball* b) {
	ball_collided_bounds(b);

	b->prev_pos.x = b->pos.x;
	b->prev_pos.y = b->pos.y;
	b->pos.x += to_meters(b->vel.x);
	b->pos.y += to_meters(b->vel.y);

}

void update_balls(void(*func)(Ball*), Ball* balls, int count)
{
	for (int i = 0; i < count; i++)
	{
		func(&(balls[i]));
		for (int j = i + 1; j < count; j++)
			ball_collided(&(balls[i]), &(balls[j]));
	}
}


// Utilities
void print_ball(Ball ball)
{
	printf(
		"Ball %d: pos = { %f, %f }, vel = { %f, %f }, c = { %f, %f, %f }\n",
		ball.id,
		ball.pos.x, ball.pos.y,
		ball.vel.x, ball.vel.y,
		ball.c.r, ball.c.g, ball.c.b
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

Ball* generate_balls(int count)
{
	Ball* balls = (Ball*) calloc(count, sizeof(Ball));
	for (int i = 0; i < count; i++) {
		balls[i] = generate_ball(i + 1);
		print_ball(balls[i]);
	}

	return balls;
}

Ball* add_balls(Ball* balls, int *original_count, int new_count)
{
	int original = *original_count;

	Ball* new_balls = (Ball*) calloc( (original + new_count), sizeof(Ball));
	if (new_balls == NULL)
	{
		printf("Error: realloc failed\n");
		return balls;
	}

	for (int i = 0; i < original; i++)
		new_balls[i] = balls[i];

	for (int i = original; i < new_count; i++)
	{
		new_balls[i] = generate_ball(i + 1);
		print_ball(new_balls[i]);
	}

	*original_count = new_count;
	return new_balls;
}
