#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cpu_conexion.h>

int main(void) {
  configuracion* configuracion = obtenerConfiguracion();

  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, &montar_servidor, configuracion);
  pthread_t hiloCliente;
  pthread_create(&hiloCliente, NULL, &conectar_con_memoria, configuracion);

  pthread_join(hiloCliente, NULL);
  pthread_join(hiloServer, NULL);
  // Hay que hacer free de los strings?
  free(configuracion);
  return EXIT_SUCCESS;
}
