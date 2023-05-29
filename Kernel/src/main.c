#include <stdio.h>
#include <kernel_config.h>
#include <pthread.h>
#include <conexion_kernel.h>

int main(void) {
  CONFIG_KERNEL = obtenerConfiguracion();
  pthread_t hiloServerKernel;
  pthread_t hiloClienteMemoria;
  pthread_t hiloClienteCpu;

  pthread_create(&hiloClienteMemoria, NULL, (void*)conectar_con_memoria, CONFIG_KERNEL);
  pthread_create(&hiloClienteCpu, NULL, (void*)conectar_con_cpu, CONFIG_KERNEL);
  pthread_create(&hiloServerKernel, NULL, (void*)montar_servidor, CONFIG_KERNEL);

  pthread_join(hiloClienteMemoria, NULL);
  pthread_join(hiloClienteCpu, NULL);

  iniciarColas();
  iniciarSemaforos();
  comenzarPlanificador();
  pthread_join(hiloServerKernel, NULL);

  free(CONFIG_KERNEL);
  return EXIT_SUCCESS;
}

