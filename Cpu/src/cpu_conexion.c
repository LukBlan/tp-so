#include <commons/string.h>
#include <conexiones.h>
#include <cpu_conexion.h>
#include <recursos.h>
#include <recursos.h>

void montarServidor() {
  t_configuracion* config = recursosCpu->configuracion;
  t_log* logger = recursosCpu->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);;

  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
  close(socketServidor);
}

void conectarConMemoria() {
  t_configuracion* config = recursosCpu->configuracion;
  //t_log* logger = recursosModulo->logger;
  int socketCliente = crearConexionServidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);
  close(socketCliente);
}

