#include <stdlib.h>
#include <conexiones.h>
#include <memory_conexiones.h>
#include <recursos.h>

void cargarConexiones() {
  t_configuracion* config = recursosMemoria->configuracion;
  t_log* logger = recursosMemoria->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor Memoria...");

  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor Memoria.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Socket del Servidor Memoria Creado Exitosamente.");
  recursosMemoria->conexiones->socketMemoria = socketServidor;
}

void montarServidor() {
  int socketServidor = recursosMemoria->conexiones->socketMemoria;
  t_log* logger = recursosMemoria->logger;

  log_info(logger, "Servidor Memoria se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
}
