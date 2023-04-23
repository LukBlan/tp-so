#include <stdio.h>
#include <stdlib.h>
#include "../include/cpu_config.h"

int main(void) {
  infoConexion infoConexionKernel = obtenerInfoDeConexion("KERNEL");
  infoConexion infoConexionMemoria = obtenerInfoDeConexion("MEMORIA");
	return EXIT_SUCCESS;
}
