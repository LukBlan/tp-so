#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cpu_conexion.h>

void montar_servidor() {
  configuracion configuracion = obtenerConfiguracion();
  int *datos = malloc(sizeof(int));
  int socketServidor = generar_servidor(configuracion.IP_ESCUCHA, configuracion.PUERTO_ESCUCHA);

  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    recv(socketCliente, datos, sizeof(int), 0);
    printf("%d\n", *datos);
    close(socketCliente);
  }
  free(datos);

}

int main(void) {
  montar_servidor();
  return EXIT_SUCCESS;
}
