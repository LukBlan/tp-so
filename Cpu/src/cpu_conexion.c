#include <commons/string.h>
#include <cpu_conexion.h>
#include <cpu_config.h>
#include <socket/servidor.h>
#include <socket/cliente.h>
#include <cpu_config.h>

void* montar_servidor(void* args) {
  configuracion config = *(configuracion*) args;
  int *datos = malloc(sizeof(int));
  int socketServidor = iniciar_servidor(config.IP_ESCUCHA, string_itoa(config.PUERTO_ESCUCHA));;

  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    recv(socketCliente, datos, sizeof(int), 0);
    printf("%d\n", *datos);
    close(socketCliente);
  }
  free(datos);
}

void* conectar_con_memoria(void* args) {
  configuracion config = *(configuracion*) args;
  int *datos = malloc(sizeof(int));
  *datos = 3;
  int socketCliente = crear_conexion_servidor(config.IP_MEMORIA, string_itoa(config.PUERTO_MEMORIA));
  send(socketCliente, datos, sizeof(int), 0);
  free(datos);
  close(socketCliente);
}

