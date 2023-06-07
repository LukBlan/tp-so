#include <serializacion/paquete.h>
#include <serializacion/buffer.h>
#include <serializacion/instrucciones.h>
#include <serializacion/paquete.h>
#include <estructuras.h>
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


void enviarInstrucciones(t_list* instrucciones) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;
  int tamanioBytesBuffer = tamanioBytesInstrucciones(instrucciones);
  t_buffer* buffer = generarBuffer(tamanioBytesBuffer);

  serializarInstrucciones(buffer, instrucciones, posicion);
  t_paquete* paquete = crearPaquete(buffer, LINEAS_INSTRUCCION);

  log_info(recursosConsola->logger, "Enviando lista de instrucciones al servidor Kernel.");
	enviar_paquete(paquete, recursosConsola->conexiones->socketKernel);
	liberarPaquete(paquete);
}

