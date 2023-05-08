#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(void) {
  configuracion configuracion = obtenerConfiguracion();
  printf("%d\n", configuracion.PUERTO_ESCUCHA);
  printf("%s\n", configuracion.IP_MEMORIA);
  printf("%d\n", configuracion.RETARDO_INSTRUCCION);
  printf("%d\n", configuracion.TAM_MAX_SEGMENTO);
  printf("%d\n", configuracion.PUERTO_ESCUCHA);
  return EXIT_SUCCESS;
}
