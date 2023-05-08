#include <commons/string.h>
#include <cpu_conexion.h>
#include <cpu_config.h>
#include <socket/servidor.h>
#include <socket/cliente.h>

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

void* conectar_con_servidor() {
  puts("!!!!");
  configuracion configuracion = obtenerConfiguracion();
  int *datos = malloc(sizeof(int));
  *datos = 3;
  int socketCliente = crear_conexion_servidor(configuracion.IP_ESCUCHA, "9001");
  send(socketCliente, datos, sizeof(int), 0);
  free(datos);
  close(socketCliente);
}

