#include <serializacion/instrucciones.h>
#include <conexiones.h>
#include <string.h>


void serializarListaInstrucciones(t_buffer* buffer, t_list* instrucciones, int cantDeInstrucciones) {
  int posicion = 0;
  memcpy(buffer->stream, &(cantDeInstrucciones), sizeof(int));
  posicion += sizeof(int);

  for(int i = 0; i < cantDeInstrucciones; i++) {
    // Nombre Instruccion
    t_instruccion *linea = list_get(instrucciones, i);
    memcpy(buffer->stream + posicion, &(linea->longitudIdentificador), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->identificador, linea->longitudIdentificador);
    posicion += linea->longitudIdentificador;

    // Primer Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[0]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[0], linea->longitudParametros[0]);
    posicion += linea->longitudParametros[0];

    // Segundo Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[1]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[1], linea->longitudParametros[1]);
    posicion += linea->longitudParametros[1];

    // Tercer Parametro
    memcpy(buffer->stream + posicion, &(linea->longitudParametros[2]), sizeof(int));
    posicion += sizeof(int);
    memcpy(buffer->stream + posicion, linea->parametros[2], linea->longitudParametros[2]);
    posicion += linea->longitudParametros[2];
  }
}

t_list* generarListaDeInstrucciones(int socketCliente) {
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
