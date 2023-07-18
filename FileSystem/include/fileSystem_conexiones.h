#ifndef CPU_CONEXION_H_
#define CPU_CONEXION_H_

  void cargarConexiones();
  void conectarConMemoria();
  void generarServidorFileSystem();
  void montarServidor();
  void realizarHandshakeMemoria();
  void procesarOperacionRecibida(int socketCliente);
  void procesarOperacion(op_code codigoOperacion, int socketCliente);
  void manejarConexion(int socketCliente);
#endif
