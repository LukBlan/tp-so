#include <serializacion/tablaDeSegmentos.h>
#include <serializacion/contexto.h>
#include <stdlib.h>
#include <string.h>

void serializarTablaDeSegmentos(t_buffer* buffer, t_list* tablaDeSegmentos) {
  int cantidadDeProcesos = tablaDeSegmentos->elements_count;
  int posicion = 0;

  memcpy(buffer->stream + posicion, &(cantidadDeProcesos), sizeof(int));
  posicion += sizeof(int);

  for(int i = 0; i < cantidadDeProcesos; i++) {
    tablaDeSegmento* segmentosProceso = list_get(tablaDeSegmentos, i);

    memcpy(buffer->stream + posicion, &(segmentosProceso->id), sizeof(int));
    posicion += sizeof(int);

    serializarSegmentos(buffer, segmentosProceso->segmentos_proceso, &posicion);
  }
}

t_list* deserializarTablaDeSegmentos(t_buffer* buffer) {
  t_list* tablaDeSegmentos = list_create();
  int cantidadDeProcesos;
  int posicion = 0;

  memcpy(&(cantidadDeProcesos), buffer->stream + posicion, sizeof(int));
  posicion += sizeof(int);

  for(int i = 0; i < cantidadDeProcesos; i++) {
    tablaDeSegmento* segmentosProceso = malloc(sizeof(tablaDeSegmento));

    memcpy(&(segmentosProceso->id), buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    segmentosProceso->segmentos_proceso = deserializarSegmentos(buffer, &posicion);
    list_add(tablaDeSegmentos, segmentosProceso);
  }
  return tablaDeSegmentos;
}

int tamanioBytesTablaDeSegmentos(t_list* tablaDeSegmentos) {
  int cantidadDeProceso = tablaDeSegmentos->elements_count;
  int tamanioTabla = sizeof(int); // cantidad de procesos

  for (int i = 0; i < cantidadDeProceso; i++) {
    tablaDeSegmento* segmentosProceso = list_get(tablaDeSegmentos, i);
    tamanioTabla += sizeof(int); // id proceso
    tamanioTabla += tamanioBytesSegmentos(segmentosProceso->segmentos_proceso);
  }
  return tamanioTabla;
}
