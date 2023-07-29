#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

int main(void) {
  int cantidadBytes = bitsToBytes(8);
	printf("numero %d\n", cantidadBytes);
	return EXIT_SUCCESS;
}
