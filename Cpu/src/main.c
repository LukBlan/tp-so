#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(void) {
  configuracion configuracion = obtenerConfiguracion();
  printf("%s", configuracion.IP_ESCUCHA);
  return EXIT_SUCCESS;
}
