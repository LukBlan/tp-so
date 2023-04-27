#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/conexion.h"
#include "../include/logger.h"
#include <pthread.h>

void* aceptarClientes(void* socketServidor) {
  int valor = *(int*)socketServidor;
  printf("%d", valor);
  int nuevoSocket = accept(valor, NULL, NULL);
}

int main(void) {
  int socketServidor = crearServidor("MEMORIA");
  pthread_t hilo;
  pthread_create(&hilo, NULL, &aceptarClientes, &socketServidor);
  int socketCliente = conectarConServidor("MEMORIA");
  close(socketCliente);
  return EXIT_SUCCESS;
}
