#ifndef CONTEXTO_H
#define CONTEXTO_H

  int tamanioBytesContexto(contextoEjecucion* contexto);
  void serializarProgramCounter(t_buffer* buffer, int programCounter, int* posicion);
  int deserializarProgramCounter(t_buffer* buffer, int* posicion);
  void serializarContexto(t_buffer* buffer, contextoEjecucion* contexto);
  contextoEjecucion* deserializarContexto(t_buffer* buffer);

#endif
