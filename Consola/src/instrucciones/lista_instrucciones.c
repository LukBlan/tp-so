#include <instrucciones/lista_instrucciones.h>
#include <instrucciones/instruccion.h>
#include <stdlib.h>

t_list* generarListaDeInstrucciones() {
  t_list* instrucciones = list_create();
  FILE* archivoPseudoCodigo = recursosConsola->archivoPseudoCodigo;

  while (!feof(recursosConsola->archivoPseudoCodigo)){
    t_instruccion *instruccion = leer_instruccion(archivoPseudoCodigo);
    list_add(instrucciones, instruccion);
  }
  return instrucciones;
}
