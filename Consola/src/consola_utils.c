#include <consola_utils.h>

void checkearArgumentosMain(t_log* logger, int cantidadArgumentos) {
  if (cantidadArgumentos != 3) {
    log_error(logger,"No se ingreso la cantidad de argumentos necesaria");
    exit(EXIT_FAILURE);
  }
}

int calcularTamanio(t_list* instrucciones, int cantidadDeInstrucciones) {
  // empieza en sizeof(int) por la cantidad de instrucciones
  int tamanioTotal = sizeof(int);
  for (int i = 0; i < cantidadDeInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    tamanioTotal += instruccion->longitudIdentificador;
    tamanioTotal += instruccion->longitudParametros[0];
    tamanioTotal += instruccion->longitudParametros[1];
    tamanioTotal += instruccion->longitudParametros[2];
    tamanioTotal += sizeof(int) * 4;
  }
  return tamanioTotal;
}

t_paquete* enpaquetarInstrucciones(t_list* instrucciones) {
  int cantidadDeInstrucciones = list_size(instrucciones);
  int tamanioTotalInstrucciones = calcularTamanio(instrucciones, cantidadDeInstrucciones);

  t_buffer* buffer = generarBuffer(tamanioTotalInstrucciones);
  serializarListaInstrucciones(buffer, instrucciones, cantidadDeInstrucciones);
  t_paquete* paquete = generarPaquete(buffer, LINEAS_INSTRUCCION);
  return paquete;
}

void liberarInstrucciones(t_list* instrucciones) {
  int tamanio = list_size(instrucciones);
  for (int i = 0; i < tamanio; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    printf("%s\n", instruccion->identificador);
    free(instruccion->identificador);
    free(instruccion);
  }
  list_destroy(instrucciones);
}

t_buffer* generarBuffer(int tamanio) {
  t_buffer* buffer = malloc(tamanio + sizeof(int));
  buffer->stream = malloc(tamanio);;
  buffer->size = tamanio;
  return buffer;
}

void serializarListaInstrucciones(t_buffer* buffer, t_list* instrucciones, int cantDeInstrucciones) {
  int posicion = 0;
  memcpy(buffer->stream, &(cantDeInstrucciones), sizeof(int));
  posicion += sizeof(int);

  for(int i = 0; i < cantDeInstrucciones; i++) {
    // Nombre Instruccion
    t_instruccion *linea = list_get(instrucciones, i);
    memcpy(buffer->stream + posicion, &(linea->longitudIdentificador), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->identificador, linea->longitudIdentificador);
    posicion += linea->longitudIdentificador;

    // Primer Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[0]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[0], linea->longitudParametros[0]);
    posicion += linea->longitudParametros[0];

    // Segundo Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[1]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[1], linea->longitudParametros[1]);
    posicion += linea->longitudParametros[1];

    // Tercer Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[2]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[2], linea->longitudParametros[2]);
    posicion += linea->longitudParametros[2];
  }
}

t_paquete* generarPaquete(t_buffer* buffer, op_code codigoOperacion) {
  t_paquete* paquete = malloc(buffer->size + sizeof(int) * 2);
  paquete->buffer = buffer;
  paquete->codigo_operacion = codigoOperacion;
  return paquete;
}

void liberarPaquete(t_paquete* paquete) {
  free(paquete->buffer->stream);
  free(paquete->buffer);
  free(paquete);
}

int generarConexionConKernel(t_recursos* recursosConsola) {
  t_configuracion* configuracion = recursosConsola->configuracion;
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



