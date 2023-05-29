#ifndef UTILS_H
#define UTILS_H

  #include <commons/string.h>
  #include <commons/txt.h>
  #include <recursos.h>
  #include <socket/cliente.h>

  void checkearArgumentosMain(t_log* logger, int cantidadArgumentos);
  int generarConexionConKernel(t_recursos* recursosConsola);
  t_paquete* enpaquetarInstrucciones(t_list* instrucciones);
  t_buffer* generarBuffer(int tamanio);
  void serializarListaInstrucciones(t_buffer* buffer, t_list* instrucciones, int cantidadDeInstrucciones);
  t_paquete* generarPaquete(t_buffer* buffer, op_code codigoOperacion);

#endif
