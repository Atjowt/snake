#include <stdio.h>
#include <stdlib.h>

#define WIDTH 8
#define HEIGHT 8

char screen[HEIGHT][WIDTH];

void clear(void) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < HEIGHT; x++) {
			screen[y][x] = '.';
		}
	}
}

void render(void) {
	system("clear");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < HEIGHT; x++) {
			fputc(screen[y][x], stderr);
			fputc(' ', stderr);
		}
		fputc('\n', stderr);
	}
}

int main(void) {

	printf("%d %d\n", WIDTH, HEIGHT);
	printf("-1 -1\n");

	while (1) {

		clear();

		int length;
		scanf("%d", &length);
		if (length == 0) break;

		for (int i = 0; i < length; i++) {
			int x, y;
			scanf("%d %d", &x, &y);
			screen[y][x] = 's';
		}

		int x, y;
		scanf("%d %d", &x, &y);
		screen[y][x] = 'a';

		render();

		int c = getchar();
		printf("%c\n", c);
	}

	return 0;
}
