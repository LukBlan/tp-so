#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

  #include <estructuras.h>

  void serializarListaInstrucciones(t_buffer* buffer, t_list* instrucciones, int cantDeInstrucciones);
  t_list* generarListaDeInstrucciones(int socketCliente);

#endif
