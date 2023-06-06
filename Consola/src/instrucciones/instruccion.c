#include <instrucciones/instruccion.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <recursos.h>

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

void generarInstruccion(char* lineaInstruccion, t_instruccion* instruccion) {
  int cantidadParametros = calcularCantidadParametros(lineaInstruccion);
  inicializarInstruccion(instruccion, lineaInstruccion, cantidadParametros);
}
