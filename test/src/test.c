#include <stdio.h>
#include <stdlib.h>

void ejemplo(int* numero) {
  (*numero) = 3;
}

int main(void) {
  int numero = 0;
	ejemplo(&numero);
	printf("%d", numero);
	return 0;
}
