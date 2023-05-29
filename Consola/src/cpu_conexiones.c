#include <cpu_conexiones.h>
#include <conexiones.h>

int generarConexionConKernel(t_recursos* recursosConsola) {
  t_configuracion* configuracion = recursosConsola->configuracion;
  t_log* logger = recursosConsola->logger;

  int socketKernel = crearConexionServidor(configuracion->IP_KERNEL, configuracion->PUERTO_KERNEL);
  log_info(logger, "Conectando con el Servidor Kernel...");

  if (socketKernel < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
    return -1;
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  return socketKernel;
}
