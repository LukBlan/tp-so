#include <serializacion/instrucciones.h>
#include <serializacion/buffer.h>
#include <commons/string.h>
#include <conexiones.h>
#include <estructuras.h>
#include <kernel_conexiones.h>
#include <planificacion.h>
#include <netdb.h>
#include <utils.h>
#include <pthread.h>
#include <serializacion/paquete.h>
#include <serializacion/contexto.h>

int idProceso = 0;

void agregarANew(PCB* pcb) {
  queue_push(colaNew, pcb);
  log_info(recursosKernel->logger, "Proceso [%d] se movio a Nuevo", pcb->pid);
  cambiarEstado(NEW, pcb);
  sem_post(&largoPlazo);
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

t_list* obtenerListaInstruciones(int socketCliente) {
  t_list* listaInstrucciones;
  int *posicion = malloc(sizeof(int));
  *posicion = 0;

  obtenerCodigoOperacion(socketCliente);
  t_buffer* buffer = obtenerBuffer(socketCliente);
  listaInstrucciones = deserializarInstrucciones(buffer, posicion);

  liberarBuffer(buffer);
  free(posicion);
  return listaInstrucciones;
}

void agregarConsolaALista(PCB* pcb, int socketCliente) {
  procesoConsola* nuevoProcesoConsola = malloc(sizeof(procesoConsola));
  nuevoProcesoConsola->pid = pcb->pid;
  nuevoProcesoConsola->socketConsola = socketCliente;
  list_add(recursosKernel->conexiones->procesosConsola, nuevoProcesoConsola);
}

void recibirSegementoMemoria(PCB* pcb) {
  int posicion = 0;
  int socketMemoria = recursosKernel->conexiones->socketMemoria;
  t_buffer* buffer = generarBuffer(4);
  t_paquete* paquete = crearPaquete(buffer, Pcb);

  enviar_paquete(paquete, socketMemoria);
  obtenerCodigoOperacion(socketMemoria);
  t_buffer* bufferRecibido = obtenerBuffer(socketMemoria);
  pcb->contexto->tablaSegmentos = deserializarSegmentos(bufferRecibido, &posicion);
  liberarPaquete(paquete);
  liberarBuffer(bufferRecibido);
}

void terminoProceso() {

}

void montarServidor() {
  t_list* instrucciones;
  PCB* pcb;
  t_log* logger = recursosKernel->logger;
  int socketServidor = recursosKernel->conexiones->socketKernel;

  log_info(logger, "Servidor Kernel se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    instrucciones = obtenerListaInstruciones(socketCliente);
    mostrarInstrucciones(instrucciones);
    pcb = crearPcb(instrucciones);
    pthread_mutex_lock(&operandoConMemoria);
    recibirSegementoMemoria(pcb);
    pthread_mutex_unlock(&operandoConMemoria);
    agregarConsolaALista(pcb, socketCliente);
    cantidadDeProcesos++;
    agregarANew(pcb);
  }
}

PCB* crearPcb(t_list* listaInstrucciones) {
  PCB* pcb = malloc(sizeof(PCB));
  memset(pcb, 0, sizeof(PCB));
  pcb->contexto = malloc(sizeof(contextoEjecucion));
  memset(pcb->contexto, 0, sizeof(contextoEjecucion));

  pthread_mutex_lock(&mutexNumeroProceso);
  pcb->pid = idProceso++;
  pthread_mutex_unlock(&mutexNumeroProceso);

  strcpy(pcb->contexto->registros.AX, "Add");
  pcb->estado = -1;
  pcb->estimadoRafaga = recursosKernel->configuracion->ESTIMACION_INICIAL;
  pcb->llegadaReady = 0;
  pcb->contexto->programCounter = 0;
  pcb->contexto->instrucciones = listaInstrucciones;
  pcb->contexto->archivosAbiertos = list_create();
  return pcb;
}

// TODO agregar tabla de segmentos
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
