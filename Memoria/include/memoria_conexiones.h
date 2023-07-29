#ifndef MEMORY_CONEXIONES_H_
#define MEMORY_CONEXIONES_H_

  void cargarConexiones();
  void montarServidor();
  void enviarSegmentoCero(int socketCliente);
  Segmento* generarSegmentoAuxiliar(Segmento* segmentoNuevo);
  void agregarSegmentoATabla(Segmento* segmentoNuevo, int idProceso);
  void agregarSegmentoAContexto(contextoEjecucion* contexto, Segmento* segmentoNuevo);
  int obtenerPosicionSegmento(contextoEjecucion* contexto, int idSeg);
  void eliminarSegmentoDeTabla(int idProceso, int posicionEnContexto);
  void eliminarSegmentoDeContexto(contextoEjecucion* contexto, int posicionEnContexto);
  void eliminarSegmentoDelArray(Segmento* segmento);
  void elimnarSegmentoDeBitArray(contextoEjecucion* contexto, int posicionEnContexto);
  void eliminarSegmentosDeProceso(int procesoId);
  void procesarOperacion(op_code codigoOperacion, int socketCliente);
  void manejarConexion(int socketCliente);
  void procesarOperacionRecibida(int socketCliente);

#endif

