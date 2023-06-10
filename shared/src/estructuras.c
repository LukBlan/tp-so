#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void liberarInstrucciones(t_list* instrucciones) {
  int numeroInstrucciones = list_size(instrucciones);

  for (int i = 0; i < numeroInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    for (int i = 0; i < instruccion->cantidadParametros; i++) {
      free(instruccion->strings[i]);
    }
    free(instruccion);
  }
  list_destroy(instrucciones);
}
