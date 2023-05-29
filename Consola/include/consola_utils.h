#ifndef UTILS_H
#define UTILS_H

  #include <recursos.h>
  #include <estructuras.h>

  //void checkearArgumentosMain(t_log* logger, int cantidadArgumentos);s
  t_paquete* enpaquetarInstrucciones(t_list* instrucciones);
  t_buffer* generarBuffer(int tamanio);
  void serializarListaInstrucciones(t_buffer* buffer, t_list* instrucciones, int cantidadDeInstrucciones);
  t_paquete* generarPaquete(t_buffer* buffer, op_code codigoOperacion);
  int calcularTamanio(t_list* instrucciones, int cantidadDeInstrucciones);
  void liberarPaquete(t_paquete* paquete);

#endif
