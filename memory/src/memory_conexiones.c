#include <stdlib.h>
#include <conexiones.h>
#include <memory_conexiones.h>
#include <recursos.h>

void montarServidor() {
  t_configuracion* config = recursosMemoria->configuracion;
  t_log* logger = recursosMemoria->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Iniciando Servidor Memoria...");

  if (socketServidor < 0) {
    log_error(logger, "Error intentando iniciar Servidor Memoria.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Servidor Memoria iniciado correctamente.");
  recursosMemoria->socketServidor = socketServidor;

  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
}
