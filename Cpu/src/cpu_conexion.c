#include <commons/string.h>
#include <cpu_conexion.h>
#include <cpu_recursos.h>
#include <cpu_recursos.h>
#include <socket/servidor.h>
#include <socket/cliente.h>

void* montar_servidor(void* args) {
  recursos recursosModulo = *(recursos*) args;
  configuracion* config = recursosModulo.configuracion;
  t_log* logger = recursosModulo.logger;
  int socketServidor = iniciar_servidor(config->IP_ESCUCHA, string_itoa(config->PUERTO_ESCUCHA));;

  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
}

void* conectar_con_memoria(void* args) {
  recursos recursosModulo = *(recursos*) args;
  configuracion* config = recursosModulo.configuracion;
  t_log* logger = recursosModulo.logger;
  int socketCliente = crear_conexion_servidor(config->IP_MEMORIA, string_itoa(config->PUERTO_MEMORIA));
  close(socketCliente);
}

