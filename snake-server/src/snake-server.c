#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8000

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

	int server_fd, new_socket;
	int opt = 1;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		perror("Setsockopt failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) == -1) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d...\n", PORT);

	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) == -1) {
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	// if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
	// 	perror("Invalid/unsupported address");
	// 	exit(EXIT_FAILURE);
	// }

	// if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr) == -1)) {
	// 	perror("Connection failed");
	// 	exit(EXIT_FAILURE);
	// }

	srand(time(NULL));

	printf("Waiting for width...\n");
	read(new_socket, &width, sizeof(int));

	printf("Waiting for height...\n");
	read(new_socket, &height, sizeof(int));

	snake = malloc(width * height * sizeof(point_t));

	printf("Waiting for x...\n");
	read(new_socket, &snake[0].x, sizeof(int));
	if (snake[0].x == -1) snake[0].x = rand() % width;

	printf("Waiting for y...\n");
	read(new_socket, &snake[0].y, sizeof(int));
	if (snake[0].y == -1) snake[0].y = rand() % height;

	alive = 1;
	length = 1;
	place_food();
	while (alive) {
		printf("Sending length...\n");
		send(new_socket, &length, sizeof(int), 0);
		for (int i = 0; i < length; i++) {
			printf("Sending segment #%d x...\n", i);
			send(new_socket, &snake[i].x, sizeof(int), 0);
			printf("Sending segment #%d y...\n", i);
			send(new_socket, &snake[i].y, sizeof(int), 0);
		}
		printf("Sending food x...\n");
		send(new_socket, &food.x, sizeof(int), 0);
		printf("Sending food y...\n");
		send(new_socket, &food.y, sizeof(int), 0);

		printf("Waiting for direction...\n");
		read(new_socket, &direction, sizeof(int));
		move_snake();
	}
	send(new_socket, &(int){0}, sizeof(int), 0);
	free(snake);
	close(new_socket);
	close(server_fd);
	return 0;
}
