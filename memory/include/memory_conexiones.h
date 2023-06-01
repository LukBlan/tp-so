#ifndef MEMORY_CONEXIONES_H_
#define MEMORY_CONEXIONES_H_

  void cargarConexiones();
  void montarServidor();
  void manejarConexion(int socketCliente);
  void procesarOperacionRecibida(int socketCliente);

#endif

