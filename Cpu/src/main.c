#include <stdio.h>
#include <stdlib.h>

#include "../include/config.h"
#include "../include/logger.h"

int main(void) {
  infoConexion infoConexionCpu = obtenerInfoDeConexion("CPU");
  infoConexion infoConexionMemoria = obtenerInfoDeConexion("MEMORIA");
  t_log* logger = crearLogger("Cpu");
  log_info(logger, "test");
  log_destroy(logger);
  return EXIT_SUCCESS;
}
