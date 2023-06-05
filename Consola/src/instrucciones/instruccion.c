#include <instrucciones/instruccion.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>

int cantidadDeLineas(char* instrucciones) {
  int cantidadDeLetras = string_length(instrucciones);
  int cantidadDeLineas = 1;

  for (int i = 0; i < cantidadDeLetras; i++) {
    if (instrucciones[i] == '\n') {
      cantidadDeLineas++;
    }
  }
  return cantidadDeLineas;
}

int calcularCantidadParametros(char* lineaInstruccion) {
  int cantidadLetras = string_length(lineaInstruccion);
  int cantidadParametros = 0;

  for (int i = 0; i <= cantidadLetras; i++) {
    if (lineaInstruccion[i] == ' ') {
      cantidadParametros++;
    }
  }
  return cantidadParametros;
}

void inicializarInstruccion(t_instruccion* instruccion, char* lineaInstruccion, int cantidadParametros) {
  int cantidadLetras = string_length(lineaInstruccion);
  int cantidadEspacios = 0;
  int start = 0;
  int end = 0;
  int cantidadDeChars = 0;

  for (int i = 0; i <= cantidadLetras; i++) {
    cantidadDeChars++;
    if (lineaInstruccion[i] == ' ' || lineaInstruccion[i] == '\0') {
      char* nuevoString = malloc(cantidadDeChars);
      for (int j = start; j < end; j++) {
        nuevoString[j - start] = lineaInstruccion[j];
      }
      nuevoString[end - start] = '\0';
      list_add(instruccion->strings, nuevoString);
      list_add(instruccion->sizeStrings, cantidadDeChars);

      cantidadEspacios++;
      start = end+1;
      cantidadDeChars = 0;
    }
    end++;
  }
}

void crearInstruccion(t_instruccion* instruccion) {
  t_list* listaStrings = list_create();
  t_list* stringsSize = list_create();
  instruccion->sizeStrings = stringsSize;
  instruccion->strings = listaStrings;
}

void generarInstrucciones(char* lineaInstruccion, t_instruccion* instruccion) {
  int cantidadParametros = calcularCantidadParametros(lineaInstruccion);
  inicializarInstruccion(instruccion, lineaInstruccion, cantidadParametros);
}
