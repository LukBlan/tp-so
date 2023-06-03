#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

  #include <estructuras.h>

  int tamanioBytesInstrucciones(t_list* instrucciones);
  void serializarInstrucciones(t_buffer* buffer, t_list* instrucciones);
  t_list* deserializarInstrucciones(int socketCliente);

#endif
