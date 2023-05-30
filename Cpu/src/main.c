#include <pthread.h>
#include <cpu_conexion.h>
#include <utils.h>

int main(int argc, char* argv[]) {
  pthread_t hiloClienteMemoria;
  pthread_t hiloServerCpu;

  validarCantidadArgumentosMain(argc, 2);
  inicializarRecursos("cpu.log", argv[1]);

  pthread_create(&hiloClienteMemoria, NULL, (void*)conectarConMemoria, NULL);
  pthread_create(&hiloServerCpu, NULL, (void*)montarServidor, NULL);

  pthread_join(hiloClienteMemoria, NULL);
  pthread_join(hiloServerCpu, NULL);

  liberarRecursos();
  return 0;
}
