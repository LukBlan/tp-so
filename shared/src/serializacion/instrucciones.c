#include <serializacion/instrucciones.h>
#include <conexiones.h>
#include <string.h>

t_list* deserializarInstrucciones(t_buffer* buffer, int* posicion) {
  t_list* instrucciones = list_create();
  int cantidadDeInstrucciones;

  memcpy(&cantidadDeInstrucciones, buffer->stream, sizeof(int));
  *posicion += sizeof(int);

  for (int i = 0; i < cantidadDeInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    int cantidadParametros;
    memcpy(&cantidadParametros, buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);

    int cantidadPalabras = cantidadParametros + 1;
    for (int j = 0; j < cantidadPalabras; j++) {
      int tamanioString;
      memcpy(&tamanioString, buffer->stream + *posicion, sizeof(int));
      *posicion += sizeof(int);

      char* string = malloc(tamanioString);
      memcpy(string, buffer->stream + *posicion, tamanioString);
      *posicion += tamanioString;
      instruccion->strings[j] = string;
    }
    instruccion->cantidadParametros = cantidadParametros;
    list_add(instrucciones, instruccion);
  }
  return instrucciones;
}

void serializarInstrucciones(t_buffer* buffer, t_list* instrucciones, int* posicion) {
  int cantidadInstrucciones = list_size(instrucciones);

  memcpy(buffer->stream, &(cantidadInstrucciones), sizeof(int));
  *posicion += sizeof(int);

  for(int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    int cantidadParametros = instruccion->cantidadParametros;

    memcpy(buffer->stream + *posicion, &(cantidadParametros), sizeof(int));
    *posicion += sizeof(int);

    int cantidadPalabras = cantidadParametros + 1;
    for (int j = 0; j < cantidadPalabras; j++) {
      int tamanioParametro = string_length(instruccion->strings[j]) + 1;
      memcpy(buffer->stream + *posicion, &(tamanioParametro), sizeof(int));
      *posicion += sizeof(int);

      memcpy(buffer->stream + *posicion, instruccion->strings[j], tamanioParametro);
      *posicion += tamanioParametro;
    }
  }
}

int tamanioBytesInstruccion(t_instruccion* instruccion) {
  int cantidadBytes = 0;
  int cantidadStrings = instruccion->cantidadParametros + 1;

  for (int i = 0; i < cantidadStrings; i++) {
    cantidadBytes += string_length(instruccion->strings[i]) + 1;
  }

  cantidadBytes += sizeof(int) * cantidadStrings;
  // sizeof(int) representando la cantidad de parametros de la instruccion
  cantidadBytes += sizeof(int);
  return cantidadBytes;
}

int tamanioBytesInstrucciones(t_list* listaInstrucciones) {
  int cantidadInstrucciones = list_size(listaInstrucciones);
  // sizeof(int) representando la cantidad de instrucciones
  int tamanioTotal = sizeof(int);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = list_get(listaInstrucciones, i);
    tamanioTotal += tamanioBytesInstruccion(instruccion);
  }

  return tamanioTotal;
}
