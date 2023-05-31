#include <commons/string.h>
#include <conexiones.h>
#include <estructuras.h>
#include <kernel_conexiones.h>
#include <planificacion.h>
#include <netdb.h>

int idProceso = 0;

void mostrar(t_list* instrucciones) {
  int tamaño = list_size(instrucciones);
  for (int i = 0; i < tamaño; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    printf("%s", instruccion->identificador);
    printf(" %s", instruccion->parametros[0]);
    printf(" %s", instruccion->parametros[1]);
    printf(" %s\n", instruccion->parametros[2]);
  }
}

void agregarANew(PCB* pcb) {
  queue_push(colaNew, pcb);
  sem_post(&largoPlazo);
  //cambiarEstado(NEW, pcb);
  //looger de instruccion
}

void crearSocketKernel() {
  t_configuracion* config = recursosKernel->configuracion;
  t_log* logger = recursosKernel->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor Kernel...");
  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor Kernel.");
    //liberarRecursos();
    exit(-1);
   }

  log_info(logger, "Socket del Servidor Kernel Creado Exitosamente.");
  recursosKernel->conexiones->socketKernel = socketServidor;
}

void montarServidor() {
  t_log* logger = recursosKernel->logger;
  int socketServidor = recursosKernel->conexiones->socketKernel;

  log_info(logger, "Servidor Kernel se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un Cliente.");
    obtener_codigo_operacion(socketCliente);
    t_list* instrucciones = generarListaDeInstrucciones(socketCliente);
    mostrar(instrucciones);
    PCB* pcb = crear_pcb(instrucciones);
    agregarANew(pcb);
    close(socketCliente);
  }
}

//TODO agregar tabla de segmentos
PCB* crear_pcb(t_list* listaInstrucciones) {
  PCB* pcb = malloc(sizeof(PCB));
  pthread_mutex_lock(&mutexNumeroProceso);
  pcb->pid = idProceso++;
  pthread_mutex_unlock(&mutexNumeroProceso);
  pcb->programCounter = 0;
  pcb->estimadoRafaga = recursosKernel->configuracion->ESTIMACION_INICIAL;
  pcb->instrucciones = list_duplicate(listaInstrucciones);
  pcb->llegadaReady = 0;
  list_destroy(listaInstrucciones);
  return pcb;
}

t_buffer* obtenerBuffer(int socketCliente) {
  int tamanioBuffer;
  recv(socketCliente, &tamanioBuffer, sizeof(int), 0);
  void* stream = malloc(tamanioBuffer);
  recv(socketCliente, stream, tamanioBuffer, 0);
  t_buffer* buffer = malloc(tamanioBuffer + sizeof(int));
  buffer->stream = stream;
  buffer->size = tamanioBuffer;
  return buffer;
}

t_list* generarListaDeInstrucciones(int socketCliente) {
  t_list* instrucciones = list_create();
  t_buffer* buffer = obtenerBuffer(socketCliente);
  int posicion = 0;

  int cantidadDeInstrucciones;

  memcpy(&cantidadDeInstrucciones, buffer->stream, sizeof(int));
  posicion += sizeof(int);
  for (int i = 0; i < cantidadDeInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    int tamanioPrimeraInstruccion;
    memcpy(&tamanioPrimeraInstruccion, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreInstruccion = string_new();
    memcpy(nombreInstruccion, buffer->stream + posicion, tamanioPrimeraInstruccion);
    posicion += tamanioPrimeraInstruccion;
    int tamañoPrimerParametro;
    memcpy(&tamañoPrimerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombrePrimerParametro = string_new();
    memcpy(nombrePrimerParametro, buffer->stream + posicion, tamañoPrimerParametro);
    posicion += tamañoPrimerParametro;

    int tamañoSegundoParametro;
    memcpy(&tamañoSegundoParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreSegundoParametro = string_new();
    memcpy(nombreSegundoParametro, buffer->stream + posicion, tamañoSegundoParametro);
    posicion += tamañoSegundoParametro;

    int tamañoTercerParametro;
    memcpy(&tamañoTercerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreTercerParametro = string_new();
    memcpy(nombreTercerParametro, buffer->stream + posicion, tamañoTercerParametro);
    posicion += tamañoTercerParametro;
    instruccion->identificador = string_duplicate(nombreInstruccion);
    instruccion->parametros[0] = nombrePrimerParametro;
    instruccion->parametros[1] = nombreSegundoParametro;
    instruccion->parametros[2] = nombreTercerParametro;
    list_add(instrucciones, instruccion);
  }
  return instrucciones;
}

void cargarConexiones() {
  conectar_con_memoria();
  conectar_con_cpu();
  conectar_con_fileSystem();
  crearSocketKernel();
}

void conectar_con_memoria() {
  t_configuracion* config = recursosKernel->configuracion;
  t_log* logger = recursosKernel->logger;

  log_info(logger, "Conectando con el Servidor Memoria...");
  int socketMemoria = crearConexionServidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);

  if (socketMemoria < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Memoria no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosKernel->conexiones->socketMemoria = socketMemoria;
}

void conectar_con_cpu() {
  t_configuracion* config = recursosKernel->configuracion;
  t_log* logger = recursosKernel->logger;

  log_info(logger, "Conectando con el Servidor Cpu...");
  int socketCpu = crearConexionServidor(config->IP_CPU, config->PUERTO_CPU);

  if (socketCpu < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Cpu no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosKernel->conexiones->socketCpu = socketCpu;
}

void conectar_con_fileSystem() {
  t_configuracion* config = recursosKernel->configuracion;
  t_log* logger = recursosKernel->logger;

  log_info(logger, "Conectando con el Servidor FileSystem...");
  int socketFileSystem = crearConexionServidor(config->IP_FILESYSTEM, config->PUERTO_FILESYSTEM);

  if (socketFileSystem < 0) {
    log_error(logger, "Conexión rechazada. El Servidor FileSystem no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosKernel->conexiones->socketFileSystem = socketFileSystem;
}


