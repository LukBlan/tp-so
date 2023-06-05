#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void liberarInstrucciones(t_list* instrucciones) {
  int numeroInstrucciones = list_size(instrucciones);

  for (int i = 0; i < numeroInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    for (int i = 0; i < list_size(instruccion->strings); i++) {
      free(list_get(instruccion->strings, i));
    }
    list_destroy(instruccion->strings);
    list_destroy(instruccion->sizeStrings);
    free(instruccion);
  }
  list_destroy(instrucciones);
}
