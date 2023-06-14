#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void liberarInstrucciones(t_list* instrucciones) {
  int numeroInstrucciones = list_size(instrucciones);

  for (int i = 0; i < numeroInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    int cantidadStrings = instruccion->cantidadParametros + 1;
    for (int i = 0; i < cantidadStrings; i++) {
      free(instruccion->strings[i]);
    }
    free(instruccion);
  }
  list_destroy(instrucciones);
}

void liberarContexto(contextoEjecucion* contexto) {
  liberarInstrucciones(contexto->instrucciones);
  free(contexto);
}
