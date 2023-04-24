#include <stdio.h>
#include <stdlib.h>

#include "../include/conexion.h"
#include "../include/logger.h"

int main(void) {
  t_log* logger = crearLogger("Cpu");
  int socket = crearSocket(logger);
  conectarConModulo(socket, "MEMORIA");
  log_destroy(logger);
  close(socket);
  return EXIT_SUCCESS;
}
