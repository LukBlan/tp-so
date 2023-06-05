#include <serializacion/instrucciones.h>
#include <commons/string.h>
#include <conexiones.h>
#include <estructuras.h>
#include <kernel_conexiones.h>
#include <planificacion.h>
#include <netdb.h>
#include <utils.h>

int idProceso = 0;

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
    obtenerCodigoOperacion(socketCliente);
    t_list* instrucciones = deserializarInstrucciones(socketCliente);
    mostrarInstrucciones(instrucciones);
    //PCB* pcb = crear_pcb(instrucciones);
    //agregarANew(pcb);
    close(socketCliente);
  }
}

//TODO agregar tabla de segmentos
PCB* crear_pcb(t_list* listaInstrucciones) {
  PCB* pcb = malloc(sizeof(PCB));
  pthread_mutex_lock(&mutexNumeroProceso);
  pcb->pid = idProceso++;
  pthread_mutex_unlock(&mutexNumeroProceso);
  pcb->contexto.programCounter = 0;
  pcb->contexto.estimadoRafaga = recursosKernel->configuracion->ESTIMACION_INICIAL;
  pcb->contexto.instrucciones = list_duplicate(listaInstrucciones);
  pcb->contexto.llegadaReady = 0;
  list_destroy(listaInstrucciones);
  return pcb;
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


