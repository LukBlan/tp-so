#include <stdlib.h>
#include "../include/conexion.h"

int crearSocket(t_log* logger) {
	int nuevoSocket;
	if ((nuevoSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	  log_error(logger, "No se pudo crear el socket correctamente");
		exit(EXIT_FAILURE);
	}
	return nuevoSocket;
}
