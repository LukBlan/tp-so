#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cpu_conexion.h>

int main(void) {
  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, &montar_servidor, NULL);
  pthread_t hiloCliente;
  pthread_create(&hiloCliente, NULL, &conectar_con_servidor, NULL);

  pthread_join(hiloCliente, NULL);
  pthread_join(hiloServer, NULL);
  return EXIT_SUCCESS;
}
