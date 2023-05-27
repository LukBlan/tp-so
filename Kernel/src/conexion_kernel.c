#include <commons/string.h>
#include <conexion_kernel.h>
#include <kernel_config.h>
#include <socket/servidor.h>
#include <socket/cliente.h>
#include <estructuras.h>

void* montar_servidor(void* args) {
  configuracion config = *(configuracion*) args;
  int socketServidor = iniciar_servidor(config.IP_ESCUCHA, string_itoa(config.PUERTO_ESCUCHA));
  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    obtener_codigo_operacion(socketCliente);
    generarListaDeInstrucciones(socketCliente);
    close(socketCliente);
  }
  close(socketServidor);
}

buffer* obtenerBuffer(int socketCliente) {
  int tamanioBuffer;
  recv(socketCliente, &tamanioBuffer, sizeof(int), MSG_WAITALL);
  void* stream = malloc(tamanioBuffer);
  recv(socketCliente, stream, tamanioBuffer, MSG_WAITALL);
  buffer* buffer = malloc(tamanioBuffer + sizeof(int));
  buffer->stream = stream;
  buffer->size = tamanioBuffer;
  return buffer;
}

t_list* generarListaDeInstrucciones(int socketCliente) {
  t_list* instrucciones = list_create();
  buffer* buffer = obtenerBuffer(socketCliente);
  int posicion = 0;

  int cantidadDeInstrucciones;

  memcpy(&cantidadDeInstrucciones, buffer->stream, sizeof(int));
  posicion += sizeof(int);
  printf("cantidad instrucciones %d\n", cantidadDeInstrucciones);

  for (int i = 0; i < cantidadDeInstrucciones; i++) {
    int tamanioPrimeraInstruccion;
    memcpy(&tamanioPrimeraInstruccion, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreInstruccion = string_new();
    memcpy(nombreInstruccion, buffer->stream + posicion, tamanioPrimeraInstruccion);
    posicion += tamanioPrimeraInstruccion;
    printf("%s ", nombreInstruccion);

    int tamañoPrimerParametro;
    memcpy(&tamañoPrimerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombrePrimerParametro = string_new();
    memcpy(nombrePrimerParametro, buffer->stream + posicion, tamañoPrimerParametro);
    printf("%s ", nombrePrimerParametro);
    posicion += tamañoPrimerParametro;

    int tamañoSegundoParametro;
    memcpy(&tamañoSegundoParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreSegundoParametro = string_new();
    memcpy(nombreSegundoParametro, buffer->stream + posicion, tamañoSegundoParametro);
    printf("%s ", nombreSegundoParametro);
    posicion += tamañoSegundoParametro;

    int tamañoTercerParametro;
    memcpy(&tamañoTercerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreTercerParametro = string_new();
    memcpy(nombreTercerParametro, buffer->stream + posicion, tamañoTercerParametro);
    printf("%s\n", nombreTercerParametro);
    posicion += tamañoTercerParametro;
    //printf("%s %s\n", nombreInstruccion, nombrePrimerParametro);

  }

  return instrucciones;
}

/*
Pcb crear_pcb(t_listlistaInstrucciones) {
  Pcb *pcb = malloc(sizeof(Pcb));
  pthread_mutex_lock(&mutexNumeroProceso);
  pcb->pid = idProceso++;
  pthread_mutex_unlock(&mutexNumeroProceso);
  pcb->programCounter = 0;
  pcb->estimadoRafaga = KERNEL_CONFIG.ESTIMACION_RAFAGA;
  pcb->instrucciones = list_duplicate(listaInstrucciones);
  pcb->llegadaReady = 0;

  list_destroy(listaInstrucciones);

  return pcb;
}
*/

void* conectar_con_memoria(void* args) {
  configuracion config = *(configuracion*) args;
  int *datos = malloc(sizeof(int));
  *datos = 3;
  int socketCliente = crear_conexion_servidor(config.IP_MEMORIA, string_itoa(config.PUERTO_MEMORIA));
  send(socketCliente, datos, sizeof(int), 0);
  free(datos);
  close(socketCliente);
}

void* conectar_con_cpu(void* args){
	configuracion config = *(configuracion*) args;
	int *datos = malloc(sizeof(int));
	*datos = 3;
	int socketClienteConCpu = crear_conexion_servidor(config.IP_CPU, string_itoa(config.PUERTO_CPU));
	send(socketClienteConCpu, datos, sizeof(int), 0);
	free(datos);
	close(socketClienteConCpu);
}


