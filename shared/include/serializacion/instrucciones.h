#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

  #include <estructuras.h>

  int tamanioBytesInstrucciones(t_list* instrucciones);
  void serializarInstrucciones(t_buffer* buffer, t_list* instrucciones, int* posicion);
  t_list* deserializarInstrucciones(t_buffer* buffer, int* posicion);
  int tamanioBytesInstruccion(t_instruccion* instruccion);
#endif
