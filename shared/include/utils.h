#ifndef UTILS_H
#define UTILS_H

  #include <commons/collections/list.h>
  #include <estructuras.h>

  void validarCantidadArgumentosMain(int cantidadArgumentos, int cantidadArgumenosEsperados);
  void mostrarInstrucciones(t_list* instrucciones);
  void mostrarContexto(contextoEjecucion* contexto);
  int bitsToBytes(int bits);
  char* generarArray(int bytes);

#endif
