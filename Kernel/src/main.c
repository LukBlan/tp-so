#include <stdio.h>
#include <kernel_config.h>
#include <pthread.h>
#include <conexion_kernel.h>

int main(void) {
  CONFIG_KERNEL = obtenerConfiguracion();

  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, (void*)montar_servidor, CONFIG_KERNEL);

  pthread_t hiloClienteMemoria;
  pthread_create(&hiloClienteMemoria, NULL, (void*)conectar_con_memoria, CONFIG_KERNEL);

  pthread_t hiloCpu;
  pthread_create(&hiloCpu, NULL, (void*)conectar_con_cpu, CONFIG_KERNEL);

  pthread_join(hiloCpu, NULL);
  pthread_join(hiloClienteMemoria, NULL);
  iniciarColas();
  iniciarSemaforos();
  comenzarPlanificador();
  pthread_join(hiloServer, NULL);

  free(CONFIG_KERNEL);
  return EXIT_SUCCESS;
}

