#include <serializacion/contexto.h>
#include <serializacion/instrucciones.h>
#include <stdlib.h>
#include <string.h>

int tamanioBytesContexto(contextoEjecucion* contexto) {
  int tamanioTotal = 0;
  tamanioTotal += tamanioBytesInstrucciones(contexto->instrucciones);
  tamanioTotal += sizeof(int) * 2;
  tamanioTotal += sizeof(t_registros);
  return tamanioTotal;
}

int tamanioBytesSegmentos(t_list* listaSegmentos) {
  int tamanioLista = listaSegmentos->elements_count;
  return tamanioLista * sizeof(Segmento) + sizeof(int);
}

void serializarSegmentos(t_buffer* buffer, t_list* segmentos, int* posicion) {
  int cantidadSegmentos = segmentos->elements_count;

  printf("cantidad de segmentos %d\n", cantidadSegmentos);
  memcpy(buffer->stream + *posicion, &(cantidadSegmentos), sizeof(int));
  *posicion += sizeof(int);

  for(int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(segmentos, i);

    memcpy(buffer->stream + *posicion, &(segmento->id), sizeof(int));
    *posicion += sizeof(int);

    memcpy(buffer->stream + *posicion, &(segmento->base), sizeof(int));
    *posicion += sizeof(int);

    memcpy(buffer->stream + *posicion, &(segmento->limite), sizeof(int));
    *posicion += sizeof(int);
  }
}

t_list* deserializarSegmentos(t_buffer* buffer, int* posicion) {
  t_list* listaSegmentos = list_create();
  int cantidadSegmentos = 0;

  memcpy(&(cantidadSegmentos), buffer->stream + *posicion, sizeof(int));
  *posicion += sizeof(int);

  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = malloc(sizeof(Segmento));

    memcpy(&(segmento->id), buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);

    memcpy(&(segmento->base), buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);

    memcpy(&(segmento->limite), buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);

    list_add(listaSegmentos, segmento);
  }

  return listaSegmentos;
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
  memcpy(buffer->stream + *posicion, registros.AX ,sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.BX ,sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.CX ,sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.DX ,sizeof(char) * 4);
  *posicion += sizeof(char) * 4;

  memcpy(buffer->stream + *posicion, registros.EAX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.EBX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.ECX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.EDX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 4;

  memcpy(buffer->stream + *posicion, registros.RAX ,sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.RBX ,sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.RCX,sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(buffer->stream + *posicion, registros.RDX,sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
}

void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  serializarInstrucciones(buffer, contexto->instrucciones, posicion);
  serializarProgramCounter(buffer, contexto->programCounter, posicion);
  serializarRegistros(buffer, contexto->registros, posicion);

  free(posicion);
}

t_registros deserializarRegistros(t_buffer* buffer, int* posicion) {
  t_registros registros;
  memcpy(registros.AX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(registros.BX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(registros.CX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;
  memcpy(registros.DX, buffer->stream + *posicion, sizeof(char) * 4);
  *posicion += sizeof(char) * 4;

  memcpy(registros.EAX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(registros.EBX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(registros.ECX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 4;
  memcpy(registros.EDX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 4;

  memcpy(registros.RAX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(registros.RBX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(registros.RCX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  memcpy(registros.RDX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 4;
  return registros;
}

contextoEjecucion* deserializarContexto(t_buffer* buffer) {
  contextoEjecucion* contexto = malloc(sizeof(contextoEjecucion));
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  contexto->instrucciones = deserializarInstrucciones(buffer, posicion);
  contexto->programCounter = deserializarProgramCounter(buffer, posicion);
  contexto->registros = deserializarRegistros(buffer, posicion);

  free(posicion);
  return contexto;
}

