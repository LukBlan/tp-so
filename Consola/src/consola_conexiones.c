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
	free(posicion);
}

void esperarResultado() {
  t_log* logger = recursosConsola->logger;
  int socketKernel = recursosConsola->conexiones->socketKernel;
  log_info(logger, "Esperando Resultado del Proceso");
  op_code resultadoEspera = obtenerCodigoOperacion(socketKernel);

  switch(resultadoEspera) {
    case SUCCESS:
      log_info(logger, "El Proceso Termino Correctamente");
      break;
    case OUT_OF_MEMORY:
      log_error(logger, "OUT_OF_MEMORY");
      break;
    case INVALID_RESOURCE:
      log_error(logger, "INVALID_RESOURCE");
      break;
    case SEGMENTATION_FAULT:
      log_error(logger, "SEGMENTATION_FAULT");
      break;
    case DESCONEXION:
      log_error(logger, "Perdi la conexion con el Kernel?");
      break;
    default:
      puts("Algo paso fijate man");
      break;
  }
}

