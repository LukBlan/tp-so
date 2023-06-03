#ifndef PAQUETE_H
#define PAQUETE_H

  t_paquete* generarPaquete(t_buffer* buffer, op_code codigoOperacion);
  void liberarPaquete(t_paquete* paquete);
  t_paquete* crear_paquete(op_code codigoOperacion);
  void* serializar_paquete(t_paquete* paquete, int bytes);
  void enviar_paquete(t_paquete* paquete, int socketCliente);
  void eliminar_paquete(t_paquete* paquete);

#endif
