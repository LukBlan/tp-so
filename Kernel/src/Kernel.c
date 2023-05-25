#include <stdio.h>
#include <kernel_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <commons/string.h>
#include <socket/servidor.h>
#include <conexion_kernel.h>

int main(void) {
  configuracion* configuracion = obtenerConfiguracion();

  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, &montar_servidor, configuracion);

  pthread_t hiloClienteMemoria;
  pthread_create(&hiloClienteMemoria, NULL, &conectar_con_memoria, configuracion);

  pthread_t hiloCliente;
  pthread_create(&hiloCliente, NULL, &conectar_con_cpu, configuracion);

  pthread_join(hiloCliente, NULL);
  pthread_join(hiloClienteMemoria, NULL);
  pthread_join(hiloServer, NULL);
  free(configuracion);

  return EXIT_SUCCESS;

}

