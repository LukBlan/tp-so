#ifndef MEMORY_CONEXIONES_H_
#define MEMORY_CONEXIONES_H_

  void cargarConexiones();
  void montarServidor();
  void manejarConexion(int socketCliente);
  void procesarOperacionRecibida(int socketCliente);
  void enviarSegmentoCero(int socketCliente);
  Segmento* generarSegmentoAuxiliar(Segmento* segmentoNuevo);
  void agregarSegmentoATabla(Segmento* segmentoNuevo, int idProceso);
  void agregarSegmentoAContexto(contextoEjecucion* contexto, Segmento* segmentoNuevo);
  int obtenerPosicionSegmento(contextoEjecucion* contexto, int idSeg);
  void eliminarSegmentoDeTabla(int idProceso, int posicionEnContexto);
  void eliminarSegmentoDeContexto(contextoEjecucion* contexto, int posicionEnContexto);
  void elimnarSegmentoDeBitArray(contextoEjecucion* contexto, int posicionEnContexto);
  void procesarOperacion(op_code codigoOperacion, int socketCliente);
#endif

