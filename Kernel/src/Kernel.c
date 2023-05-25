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
  printf("%d\n",configuracion->PUERTO_CPU);
  char* puerto = string_itoa(configuracion->PUERTO_ESCUCHA);
  int socket_servidor = iniciar_servidor(configuracion->IP_ESCUCHA,puerto);
  printf("%d\n",socket_servidor);

  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, &montar_servidor, configuracion);
  pthread_join(hiloServer, NULL);

  pthread_t hiloClienteMemoria;
  pthread_create(&hiloClienteMemoria, NULL, &conectar_con_memoria, configuracion);
  pthread_join(hiloClienteMemoria, NULL);

  pthread_t hiloCliente;
  pthread_create(&hiloCliente, NULL, &conectar_con_cpu, configuracion);
  pthread_join(hiloCliente, NULL);

  free(configuracion);

  return EXIT_SUCCESS;

}

