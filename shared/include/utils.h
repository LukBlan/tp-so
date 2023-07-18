#ifndef UTILS_H
#define UTILS_H

  #include <commons/collections/list.h>
  #include <estructuras.h>

  void validarCantidadArgumentosMain(int cantidadArgumentos, int cantidadArgumenosEsperados);
  void mostrarInstrucciones(t_list* instrucciones);
  void mostrarContexto(contextoEjecucion* contexto);
  int bitsToBytes(int bits);
  char* generarArray(int bytes);
  t_list* f_seek(char* nomArchivo, t_list* archivosAbiertos, int posicion);
  archivoAbierto* buscarNombre(t_list* archivos, char* nombreArch);
  void mostrarRegistros(t_registros registros);

#endif
