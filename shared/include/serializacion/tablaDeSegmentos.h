#ifndef TABLA_DE_SEGMENTOS_H
#define TABLA_DE_SEGMENTOS_H

  #include <serializacion/tablaDeSegmentos.h>
  #include <serializacion/contexto.h>

  void serializarTablaDeSegmentos(t_buffer* buffer, t_list* tablaDeSegmentos);
  t_list* deserializarTablaDeSegmentos(t_buffer* buffer);
  int tamanioBytesTablaDeSegmentos(t_list* tablaDeSegmentos);

#endif
