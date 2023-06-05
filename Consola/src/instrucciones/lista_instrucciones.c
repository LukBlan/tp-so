#include <commons/collections/list.h>
#include <instrucciones/lista_instrucciones.h>
#include <instrucciones/instruccion.h>
#include <stdlib.h>

void getListStrings(char* example, t_list* listaStrings) {
  int start = 0;
  int end = 0;
  int numeroLetras = string_length(example);

  for (int i = 0; i <= numeroLetras; i++) {
    if (example[i] == '\0'  || example[i] == '\n') {
      int tamanioInstruccion = end + 1 - start;
      char* newString = string_substring(example, start, tamanioInstruccion);
      newString[tamanioInstruccion-1] = '\0';
      start = end+1;
      list_add(listaStrings, newString);
    }
    end++;
  }
}

char* getFileAsString() {
  FILE* archivo = recursosConsola->archivoPseudoCodigo;
  char* instrucciones;
  char c;
  long f_size = 0;

  fseek(archivo, 0, SEEK_END);
  f_size = ftell(archivo);
  fseek(archivo, 0, SEEK_SET);
  instrucciones = malloc(f_size * sizeof(char) + 1);

  for (int i = 0; i < f_size; i++) {
   c = fgetc(archivo);
   instrucciones[i] = c;
  }

  instrucciones[f_size] = '\0';
  return instrucciones;
}

void generarListaDeInstrucciones(t_list* listaInstrucciones) {
  char* fileAsString = getFileAsString();
  t_list* listaStrings = list_create();;

  getListStrings(fileAsString, listaStrings);
  int cantidadInstrucciones = list_size(listaStrings);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    crearInstruccion(instruccion);
    generarInstrucciones(list_get(listaStrings, i), instruccion);
    free(list_get(listaStrings, i));
    list_add(listaInstrucciones, instruccion);
  }

  list_destroy(listaStrings);
  free(fileAsString);
}
