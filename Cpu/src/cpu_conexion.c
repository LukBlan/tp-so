#include <commons/string.h>
#include <cpu_conexion.h>
#include <cpu_config.h>
#include <socket/servidor.h>

void* montar_servidor() {
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

int generar_servidor(char* ip, int puerto) {
  char* puertoServidor = string_itoa(puerto);
  int socketServidor = iniciar_servidor(ip, puertoServidor);
  return socketServidor;
}

