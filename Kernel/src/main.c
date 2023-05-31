#include <kernel_conexiones.h>
#include <stdio.h>
#include <recursos.h>
#include <pthread.h>
#include <utils.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursos();
  cargarConfiguracion(argv[1]);
  cargarLogger("kernel.log");

  pthread_t hiloServerKernel;
  pthread_t hiloClienteMemoria;
  pthread_t hiloClienteCpu;

  pthread_create(&hiloClienteMemoria, NULL, (void*)conectar_con_memoria, NULL);
  pthread_create(&hiloClienteCpu, NULL, (void*)conectar_con_cpu, NULL);
  pthread_create(&hiloServerKernel, NULL, (void*)montar_servidor, NULL);

  pthread_join(hiloClienteMemoria, NULL);
  pthread_join(hiloClienteCpu, NULL);

  iniciarColas();
  iniciarSemaforos();
  comenzarPlanificador();
  pthread_join(hiloServerKernel, NULL);

  free(recursosKernel);
  return EXIT_SUCCESS;
}

