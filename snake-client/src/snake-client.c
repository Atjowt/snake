#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

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

	int socket_fd;
	struct sockaddr_in server_addr;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
		perror("Invalid/unsupported address");
		exit(EXIT_FAILURE);
	}

	if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}
	
	printf("Sending width...\n");
	send(socket_fd, &(int){WIDTH}, sizeof(int), 0);
	printf("Sending height...\n");
	send(socket_fd, &(int){HEIGHT}, sizeof(int), 0);
		

	printf("Sending x...\n");
	send(socket_fd, &(int){-1}, sizeof(int), 0);
	printf("Sending y...\n");
	send(socket_fd, &(int){-1}, sizeof(int), 0);

	while (1) {

		clear();

		int length;
			
		printf("Waiting for length...\n");
		read(socket_fd, &length, sizeof(int));
		if (length == 0) break;

		for (int i = 0; i < length; i++) {
			int x, y;
			printf("Waiting for segment #%d x...\n", i);
			read(socket_fd, &x, sizeof(int));
			printf("Waiting for segment #%d y...\n", i);
			read(socket_fd, &y, sizeof(int));
			screen[y][x] = 's';
		}

		int x, y;
		printf("Waiting for food x...\n"); 
		read(socket_fd, &x, sizeof(int));
		printf("Waiting for food y...\n"); 
		read(socket_fd, &y, sizeof(int));
		screen[y][x] = 'a';

		render();
		
		int direction;
		printf("Input direction: ");
		scanf("%d", &direction);

		printf("Sending direction...\n");
		send(socket_fd, &direction, sizeof(int), 0);
	}

	printf("Shutting down...\n");

	close(socket_fd);

	return 0;
}
