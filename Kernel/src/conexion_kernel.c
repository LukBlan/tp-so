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
    close(socketCliente);
  }
  close(socketServidor);
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


