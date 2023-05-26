#include <consola_utils.h>

void checkearArgumentosMain(t_log* logger, int cantidadArgumentos) {
  if (cantidadArgumentos != 3) {
    log_error(logger,"No se ingreso la cantidad de argumentos necesaria");
    exit(EXIT_FAILURE);
  }
}

paquete* enpaquetarInstrucciones(t_list* instrucciones) {
  paquete* paquete;

  paquete = crear_paquete(LINEAS_INSTRUCCION);
  serializarInstrucciones(paquete, instrucciones);
  return paquete;
}

int generarConexionConKernel(recursos* recursosConsola) {
  configuracion* configuracion = recursosConsola->configuracion;
  t_log* logger = recursosConsola->logger;

  int socketKernel = crear_conexion_servidor(configuracion->IP_KERNEL, configuracion->PUERTO_KERNEL);
  log_info(logger, "Conectando con el Servidor Kernel...");

  if (socketKernel < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
    return EXIT_FAILURE;
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  return socketKernel;
}



