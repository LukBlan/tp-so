#ifndef PAQUETE_H
#define PAQUETE_H

  #include <estructuras.h>

  t_paquete* crearPaquete(t_buffer* buffer, op_code codigoOperacion);
  void liberarPaquete(t_paquete* paquete);
  void* serializar_paquete(t_paquete* paquete, int bytes);

#endif
