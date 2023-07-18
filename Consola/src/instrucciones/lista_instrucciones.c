#include <commons/collections/list.h>
#include <instrucciones/lista_instrucciones.h>
#include <instrucciones/instruccion.h>
#include <stdlib.h>
#include <estructuras.h>
#include <string.h>

void filtrarListaStrings(t_list* listaStrings) {
  for (int i = 0; i < listaStrings->elements_count; i++) {
    char* string = list_get(listaStrings, i);
    if (string[0] == '\0') {
      list_remove(listaStrings, i);
      free(string);
      i--;
    }
  }
}

void generarListaDeInstrucciones(t_list* listaInstrucciones) {
  char* fileAsString = getFileAsString();
  t_list* listaStrings = list_create();

  getListStrings(fileAsString, listaStrings);
  filtrarListaStrings(listaStrings);
  int cantidadInstrucciones = list_size(listaStrings);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    char* lineaInstruccion = list_get(listaStrings, i);
    inicializarInstruccion(instruccion, lineaInstruccion);
    free(lineaInstruccion);
    list_add(listaInstrucciones, instruccion);
  }

  list_destroy(listaStrings);
  free(fileAsString);
}
