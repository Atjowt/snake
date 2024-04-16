#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct { int x, y; } point_t;

int width, height;

int alive;
int length;
int direction;
point_t* snake;
point_t food;

void place_food(void) {
	food.x = rand() % width;
	food.y = rand() % height;
	for (int i = 0; i < length; i++) {
		if (food.x == snake[i].x && food.y == snake[i].y) {
			return place_food();
		}
	}
}

void move_snake(void) {

	for (int i = length - 1; i > 0; --i) {
		snake[i] = snake[i - 1];
	}

	static int dx[4] = { 1, 0, -1, 0 };
	static int dy[4] = { 0, -1, 0, 1 };

	snake[0].x += dx[direction];
	snake[0].y += dy[direction];

	if (snake[0].x == food.x && snake[0].y == food.y) {
		snake[length] = snake[length - 1];
		length++;
		place_food();
		return;
	}

	for (int i = 1; i < length; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			alive = 0;
		}
	}
}

int main(void) {
	srand(time(NULL));
	scanf("%d %d", &width, &height);
	snake = malloc(width * height * sizeof(point_t));
	scanf("%d %d", &snake[0].x, &snake[0].y);
	if (snake[0].x == -1) snake[0].x = rand() % width;
	if (snake[0].y == -1) snake[0].y = rand() % height;
	alive = 1;
	length = 1;
	place_food();
	while (alive) {
		printf("%d\n", length);
		for (int i = 0; i < length; i++) {
			printf("%d %d\n", snake[i].x, snake[i].y);
		}
		printf("%d %d\n", food.x, food.y);
		scanf("%d", &direction);
		move_snake();
	}
	printf("%d\n", 0);
	free(snake);
	return 0;
}
