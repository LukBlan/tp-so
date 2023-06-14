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

void serializarRegistros(t_buffer* buffer, t_registros registros, int* posicion) {
  printf("test %s\n", registros.AX);
  memcpy(registros.AX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
}

void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  serializarInstrucciones(buffer, contexto->instrucciones, posicion);
  serializarProgramCounter(buffer, contexto->programCounter, posicion);
  printf("Registros %s\n", contexto->registros.AX);
  serializarRegistros(buffer, contexto->registros, posicion);

  free(posicion);
}

t_registros deserializarRegistros(t_buffer* buffer, int* posicion) {
  t_registros registros;
  memcpy(registros.AX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  printf("Registros %s\n", registros.AX);
  return registros;
}

contextoEjecucion* deserializarContexto(t_buffer* buffer) {
  contextoEjecucion* contexto = malloc(sizeof(contextoEjecucion));
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  contexto->instrucciones = deserializarInstrucciones(buffer, posicion);
  contexto->programCounter = deserializarProgramCounter(buffer, posicion);
  contexto->registros = deserializarRegistros(buffer, posicion);

  printf("Registro ax: %s ", contexto->registros.AX);
  free(posicion);
  return contexto;
}

