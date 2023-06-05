#include <serializacion/instrucciones.h>
#include <conexiones.h>
#include <string.h>

/*
t_list* deserializarInstrucciones(int socketCliente) {
  t_list* instrucciones = list_create();
  t_buffer* buffer = obtenerBuffer(socketCliente);
  int posicion = 0;

  int cantidadDeInstrucciones;

  memcpy(&cantidadDeInstrucciones, buffer->stream, sizeof(int));
  posicion += sizeof(int);
  for (int i = 0; i < cantidadDeInstrucciones; i++) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));

    int tamanioPrimeraInstruccion;
    memcpy(&tamanioPrimeraInstruccion, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreInstruccion = string_new();
    memcpy(nombreInstruccion, buffer->stream + posicion, tamanioPrimeraInstruccion);
    posicion += tamanioPrimeraInstruccion;
    int tamañoPrimerParametro;
    memcpy(&tamañoPrimerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombrePrimerParametro = string_new();
    memcpy(nombrePrimerParametro, buffer->stream + posicion, tamañoPrimerParametro);
    posicion += tamañoPrimerParametro;

    int tamañoSegundoParametro;
    memcpy(&tamañoSegundoParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreSegundoParametro = string_new();
    memcpy(nombreSegundoParametro, buffer->stream + posicion, tamañoSegundoParametro);
    posicion += tamañoSegundoParametro;

    int tamañoTercerParametro;
    memcpy(&tamañoTercerParametro, buffer->stream + posicion, sizeof(int));
    posicion += sizeof(int);

    char* nombreTercerParametro = string_new();
    memcpy(nombreTercerParametro, buffer->stream + posicion, tamañoTercerParametro);
    posicion += tamañoTercerParametro;
    instruccion->identificador = string_duplicate(nombreInstruccion);
    instruccion->parametros[0] = nombrePrimerParametro;
    instruccion->parametros[1] = nombreSegundoParametro;
    instruccion->parametros[2] = nombreTercerParametro;
    list_add(instrucciones, instruccion);
  }

  return instrucciones;
}
*/

void serializarInstrucciones(t_buffer* buffer, t_list* instrucciones) {
  int cantidadInstrucciones = list_size(instrucciones);
  int posicion = 0;

  memcpy(buffer->stream, &(cantidadInstrucciones), sizeof(int));
  posicion += sizeof(int);

  for(int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    int cantidadParametros = list_size(instruccion->strings);

    memcpy(buffer->stream, &(cantidadParametros), sizeof(int));
    posicion += sizeof(int);

    for (int j = 0; j < cantidadParametros; j++) {
      int tamanioParametro = list_get(instruccion->sizeStrings, j);
      printf("%d\n", tamanioParametro);
      memcpy(buffer->stream + posicion, &(tamanioParametro), sizeof(int));
      posicion += sizeof(int);

      memcpy(buffer->stream + posicion, list_get(instruccion->strings, j), tamanioParametro);
      posicion += tamanioParametro;
    }
  }
}

int tamanioBytesInstruccion(t_instruccion* instruccion) {
  int cantidadBytes = 0;
  int cantidadParametros = list_size(instruccion->strings);

  for (int i = 0; i < cantidadParametros; i++) {
    cantidadBytes += string_length(list_get(instruccion->strings, i)) + 1;
  }
  cantidadBytes += sizeof(int) * cantidadParametros;
  return cantidadBytes;
}

int tamanioBytesInstrucciones(t_list* listaInstrucciones) {
  int cantidadInstrucciones = list_size(listaInstrucciones);
  // sizeof(int) representando la cantidad de instrucciones
  int tamanioTotal = sizeof(int);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = list_get(listaInstrucciones, i);
    tamanioTotal += tamanioBytesInstruccion(instruccion);
    // sizeof(int) representando la cantidad de parametros de la instruccion
    tamanioTotal += sizeof(int);
  }

  return tamanioTotal;
}
