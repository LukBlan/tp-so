#ifndef CONSOLA_CONEXIONES_H
#define CONSOLA_CONEXIONES_H

  #include <commons/collections/list.h>

  void cargarConexionConKernel();
  void enviarInstrucciones(t_list* instrucciones);
  void esperarResultado();

#endif
