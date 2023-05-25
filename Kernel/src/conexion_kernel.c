#include <commons/string.h>
#include <conexion_kernel.h>
#include <kernel_config.h>
#include <socket/servidor.h>
#include <socket/cliente.h>

void* subthread(void* args) {
	configuracion config = *(configuracion*) args;
    int *datos = malloc(sizeof(int));
    int socketServidor = iniciar_servidor(config.IP_ESCUCHA, string_itoa(config.PUERTO_ESCUCHA));;

    while (1) {
    	int socketConsola = esperar_cliente(socketServidor);
    	printf("%d\n", socketConsola);
    	recv(socketConsola, datos, sizeof(int), 0);
    	printf("%d\n", *datos);
    	close(socketConsola);
      }
    free(datos);
}

void* montar_servidor(void* args) {
  configuracion config = *(configuracion*) args;
  int *datos = malloc(sizeof(int));
  int socketServidor = iniciar_servidor(config.IP_ESCUCHA, string_itoa(config.PUERTO_ESCUCHA));
  printf("%d\n", socketServidor);

  while (1) {
    int socketCliente = esperar_cliente(socketServidor);
    /**/
    pthread_t subthread_id;
    int subthread_arg = 1;
    pthread_create(&subthread_id, NULL, subthread, (void*)&subthread_arg);

    pthread_join(subthread_id, NULL);
    pthread_exit(NULL);

    printf("%d\n", socketCliente);
    close(socketCliente);
  }
  free(datos);
}

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


