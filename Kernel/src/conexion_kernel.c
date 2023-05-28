#include <commons/string.h>
#include <conexion_kernel.h>
#include <socket/servidor.h>
#include <socket/cliente.h>
#include <estructuras.h>
#include <planificacion.h>

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
  cambiarEstado(NEW, pcb);
  //looger de instruccion
}

void montar_servidor(configuracion* config) {
  int socketServidor = iniciar_servidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);
  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    obtener_codigo_operacion(socketCliente);
    t_list* instrucciones = generarListaDeInstrucciones(socketCliente);
    mostrar(instrucciones);
    PCB* pcb = crear_pcb(instrucciones);
    agregarANew(pcb);
    close(socketCliente);
  }
  close(socketServidor);
}

//TODO agregar tabla de segmentos
PCB* crear_pcb(t_list* listaInstrucciones) {
  PCB* pcb = malloc(sizeof(PCB));
  pthread_mutex_lock(&mutexNumeroProceso);
  pcb->pid = idProceso++;
  pthread_mutex_unlock(&mutexNumeroProceso);
  pcb->programCounter = 0;
  pcb->estimadoRafaga = CONFIG_KERNEL->ESTIMACION_INICIAL;
  pcb->instrucciones = list_duplicate(listaInstrucciones);
  pcb->llegadaReady = 0;
  list_destroy(listaInstrucciones);
  return pcb;
}

t_buffer* obtenerBuffer(int socketCliente) {
  int tamanioBuffer;
  recv(socketCliente, &tamanioBuffer, sizeof(int), MSG_WAITALL);
  void* stream = malloc(tamanioBuffer);
  recv(socketCliente, stream, tamanioBuffer, MSG_WAITALL);
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


void conectar_con_memoria(configuracion* config) {
  int socketCliente = crear_conexion_servidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);
  close(socketCliente);
}

void conectar_con_cpu(configuracion* config) {
	int socketClienteConCpu = crear_conexion_servidor(config->IP_CPU, config->PUERTO_CPU);
	close(socketClienteConCpu);
}


