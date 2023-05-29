#include <conexiones.h>
#include <consola_conexiones.h>
#include <recursos.h>

int generarConexionConKernel() {
  t_configuracion* config = recursosConsola->configuracion;
  t_log* logger = recursosConsola->logger;
  int* socketServer = malloc(sizeof(int));
  int socketKernel;
  int estadoConexion;

  estadoConexion = crearConexionServidor(socketServer, config->IP_KERNEL, config->PUERTO_KERNEL);
  log_info(logger, "Conectando con el Servidor Kernel...");

  if (estadoConexion < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  socketKernel = *socketServer;
  free(socketServer);
  return socketKernel;
}
