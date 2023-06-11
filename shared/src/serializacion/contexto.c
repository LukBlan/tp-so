#include <serializacion/contexto.h>
#include <serializacion/instrucciones.h>
#include <stdlib.h>
#include <string.h>

int tamanioBytesContexto(contextoEjecucion* contexto) {
  int tamanioTotal = 0;
  tamanioTotal += tamanioBytesInstrucciones(contexto->instrucciones);
  tamanioTotal += sizeof(int) * 3;
  tamanioTotal += sizeof(float);
  return tamanioTotal;
}

void serializarProgramCounter(t_buffer* buffer, int programCounter, int* posicion) {
  memcpy(buffer->stream + *posicion, &(programCounter), sizeof(int));
  *posicion += sizeof(int);
}

int deserializarProgramCounter(t_buffer* buffer, int* posicion) {
  int programCounter;
  memcpy(&(programCounter), buffer->stream + *posicion, sizeof(int));
  *posicion += sizeof(int);
  return programCounter;
}

void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  serializarInstrucciones(buffer, contexto->instrucciones, posicion);
  serializarProgramCounter(buffer, contexto->programCounter, posicion);

  free(posicion);
}

contextoEjecucion* deserializarContexto(t_buffer* buffer) {
  contextoEjecucion* contexto = malloc(sizeof(contextoEjecucion));
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  contexto->instrucciones = deserializarInstrucciones(buffer, posicion);
  contexto->programCounter = deserializarProgramCounter(buffer, posicion);
  free(posicion);
  return contexto;
}

