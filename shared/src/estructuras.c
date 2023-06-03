#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void liberarInstrucciones(t_list* instrucciones) {
  int tamanio = list_size(instrucciones);
  for (int i = 0; i < tamanio; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    printf("%s\n", instruccion->identificador);
    free(instruccion->identificador);
    free(instruccion);
  }
  list_destroy(instrucciones);
}
