#include <pthread.h>
#include <cpu_conexion.h>

int main(void) {
  recursos* recursosModulo = obtenerRecursosModulo("cpu.config", "cpu.log");
  pthread_t hiloServer;
  pthread_create(&hiloServer, NULL, (void*)montar_servidor, recursosModulo);
  pthread_t hiloCliente;
  pthread_create(&hiloCliente, NULL, (void*)conectar_con_memoria, recursosModulo);

  pthread_join(hiloCliente, NULL);
  pthread_join(hiloServer, NULL);

  liberarRecursos(recursosModulo);
  return 0;
}
