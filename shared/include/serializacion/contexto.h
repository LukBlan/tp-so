#ifndef CONTEXTO_H
#define CONTEXTO_H

  #include <estructuras.h>

  int tamanioBytesContexto(contextoEjecucion* contexto);
  void serializarProgramCounter(t_buffer* buffer, int programCounter, int* posicion);
  int deserializarProgramCounter(t_buffer* buffer, int* posicion);
  void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto);
  contextoEjecucion* deserializarContexto(t_buffer* buffer);
  int tamanioBytesSegmentos(t_list* listaSegmentos);
  void serializarSegmentos(t_buffer* buffer, t_list* segmentos, int* posicion);
  t_list* deserializarSegmentos(t_buffer* buffer, int* posicion);
  int tamanioBytesListaArchivosAbiertos(t_list* listaArchivos);
  void serializarArchivosAbiertos(t_buffer* buffer, contextoEjecucion* contexto, int* posicion);
  t_list* deserializarArchivosAbiertos(t_buffer* buffer, int* posicion);
  void serializarRegistros(t_buffer* buffer, t_registros registros, int* posicion);
  t_registros deserializarRegistros(t_buffer* buffer, int* posicion);
#endif
