#include <stdio.h>

int main(void) {
	int x;
	scanf("%d\n", &x);
	fprintf(stderr, "Received %d\n", x);
	return 0;
}
