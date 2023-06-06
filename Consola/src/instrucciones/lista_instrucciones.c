#include <commons/collections/list.h>
#include <instrucciones/lista_instrucciones.h>
#include <instrucciones/instruccion.h>
#include <stdlib.h>
#include <estructuras.h>

void generarListaDeInstrucciones(t_list* listaInstrucciones) {
  char* fileAsString = getFileAsString();
  t_list* listaStrings = list_create();;

  getListStrings(fileAsString, listaStrings);
  int cantidadInstrucciones = list_size(listaStrings);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    crearInstruccion(instruccion);
    generarInstruccion(list_get(listaStrings, i), instruccion);
    free(list_get(listaStrings, i));
    list_add(listaInstrucciones, instruccion);
  }

  list_destroy(listaStrings);
  free(fileAsString);
}
