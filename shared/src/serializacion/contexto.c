#include <serializacion/contexto.h>
#include <serializacion/instrucciones.h>
#include <stdlib.h>
#include <string.h>

int tamanioBytesContexto(contextoEjecucion* contexto) {
  int tamanioTotal = 0;
  tamanioTotal += tamanioBytesInstrucciones(contexto->instrucciones);
  tamanioTotal += sizeof(int);
  tamanioTotal += sizeof(t_registros);
  tamanioTotal += tamanioBytesListaArchivosAbiertos(contexto->archivosAbiertos);
  tamanioTotal += tamanioBytesSegmentos(contexto->tablaSegmentos);
  return tamanioTotal;
}

int tamanioBytesSegmentos(t_list* listaSegmentos) {
  int tamanioLista = listaSegmentos->elements_count;
  return tamanioLista * sizeof(Segmento) + sizeof(int);
}

int tamanioBytesListaArchivosAbiertos(t_list* listaArchivos) {
  int cantidadArchivos = listaArchivos->elements_count;
  // Por cantidadDeElementos de la lista;
  int bytes = sizeof(int);

  for (int i = 0; i < cantidadArchivos; i++) {
    archivoAbierto* archivo = list_get(listaArchivos, i);
    bytes += string_length(archivo->nombre) + 1;
    bytes += sizeof(int);
    bytes += sizeof(FILE*);
  }

  return bytes;
}

void serializarSegmentos(t_buffer* buffer, t_list* segmentos, int* posicion) {
  int cantidadSegmentos = segmentos->elements_count;

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
  printf("Deserialize segmentos %d\n", cantidadSegmentos);
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
  *posicion += sizeof(char) * 8;
  memcpy(buffer->stream + *posicion, registros.EBX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 8;
  memcpy(buffer->stream + *posicion, registros.ECX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 8;
  memcpy(buffer->stream + *posicion, registros.EDX ,sizeof(char) * 8);
  *posicion += sizeof(char) * 8;

  memcpy(buffer->stream + *posicion, registros.RAX ,sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(buffer->stream + *posicion, registros.RBX ,sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(buffer->stream + *posicion, registros.RCX,sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(buffer->stream + *posicion, registros.RDX,sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
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
  *posicion += sizeof(char) * 8;
  memcpy(registros.EBX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 8;
  memcpy(registros.ECX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 8;
  memcpy(registros.EDX, buffer->stream + *posicion, sizeof(char) * 8);
  *posicion += sizeof(char) * 8;

  memcpy(registros.RAX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(registros.RBX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(registros.RCX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  memcpy(registros.RDX, buffer->stream + *posicion, sizeof(char) * 16);
  *posicion += sizeof(char) * 16;
  return registros;
}

void serializarArchivosAbiertos(t_buffer* buffer, contextoEjecucion* contexto, int* posicion) {
  t_list* listaArchivos = contexto->archivosAbiertos;
  int cantidadArchivos = listaArchivos->elements_count;

  memcpy(buffer->stream + *posicion, &(cantidadArchivos), sizeof(int));
  *posicion += sizeof(int);
  printf("Cantidad de archivos serializados %d\n", cantidadArchivos);

  for (int i = 0; i < cantidadArchivos; i++) {
    archivoAbierto* archivo = list_get(listaArchivos, i);
    char* nombreArchivo = archivo->nombre;
    int cantidadCaracteres = string_length(nombreArchivo) + 1;

    memcpy(buffer->stream + *posicion, &(cantidadCaracteres), sizeof(int));
    *posicion += sizeof(int);

    memcpy(buffer->stream + *posicion, nombreArchivo, cantidadCaracteres);
    *posicion += cantidadCaracteres;

    printf("Nombre Archivo: %s Dirrecion %d\n",nombreArchivo, archivo->punteroArchivo);
    memcpy(buffer->stream + *posicion, &(archivo->punteroArchivo), sizeof(int));
    *posicion += sizeof(int);
  }
  puts("aa");
}

t_list* deserializarArchivosAbiertos(t_buffer* buffer, int* posicion) {
  t_list* listaArchivos = list_create();
  int cantidadArchivos = 0;

  memcpy(&(cantidadArchivos), buffer->stream + *posicion, sizeof(int));
  *posicion += sizeof(int);

  for (int i = 0; i < cantidadArchivos; i++) {
    archivoAbierto* archivo = malloc(sizeof(archivoAbierto));
    int cantidadCaracteres = 0;

    memcpy(&(cantidadCaracteres), buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);
    printf("cantidad caracteres %d", cantidadCaracteres);

    char* nombreArchivo = malloc(cantidadCaracteres);
    memcpy(nombreArchivo, buffer->stream + *posicion,  cantidadCaracteres);
    *posicion += cantidadCaracteres;

    int punteroArchivo;
    memcpy(&(punteroArchivo), buffer->stream + *posicion, sizeof(int));
    *posicion += sizeof(int);

    printf("dirrecion deserializada %d\n", punteroArchivo);
    archivo->nombre = nombreArchivo;
    archivo->punteroArchivo = punteroArchivo;
    list_add(listaArchivos, archivo);
  }

  return listaArchivos;
}

void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  serializarInstrucciones(buffer, contexto->instrucciones, posicion);
  serializarProgramCounter(buffer, contexto->programCounter, posicion);
  serializarRegistros(buffer, contexto->registros, posicion);
  serializarSegmentos(buffer, contexto->tablaSegmentos, posicion);
  serializarArchivosAbiertos(buffer, contexto, posicion);

  free(posicion);
}


contextoEjecucion* deserializarContexto(t_buffer* buffer) {
  contextoEjecucion* contexto = malloc(sizeof(contextoEjecucion));
  memset(contexto, 0, sizeof(contextoEjecucion));
  int* posicion = malloc(sizeof(int));
  *posicion = 0;

  contexto->instrucciones = deserializarInstrucciones(buffer, posicion);
  contexto->programCounter = deserializarProgramCounter(buffer, posicion);
  contexto->registros = deserializarRegistros(buffer, posicion);
  contexto->tablaSegmentos = deserializarSegmentos(buffer, posicion);
  contexto->archivosAbiertos = deserializarArchivosAbiertos(buffer, posicion);

  free(posicion);
  return contexto;
}

