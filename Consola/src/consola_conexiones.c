#include <conexiones.h>
#include <consola_conexiones.h>
#include <recursos.h>

void cargarConexionConKernel() {
  t_configuracion* config = recursosConsola->configuracion;
  t_log* logger = recursosConsola->logger;
  int socketKernel;

  socketKernel = crearConexionServidor(config->IP_KERNEL, config->PUERTO_KERNEL);
  log_info(logger, "Conectando con el Servidor Kernel...");

  if (socketKernel < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosConsola->conexiones->socketKernel = socketKernel;
}
