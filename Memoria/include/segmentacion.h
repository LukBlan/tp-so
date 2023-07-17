#ifndef SEGMENTACION_H
#define SEGMENTACION_H

  #include <recursos.h>
  #include <stdlib.h>
  #include <utils.h>
  #include <estructuras.h>
  #include <commons/bitarray.h>

  extern void* memoriaPrincipal;
  extern char* tablaDeHuecos;
  extern t_bitarray* bitMapSegmento;
  extern Segmento* segmentoCero;
  extern t_list* tablaDeSegmentosPorProceso;

  typedef struct {
    int idSegmento;
    void* contenido;
  } contenidoSegmento;

  void iniciarSegmentacion();
  Segmento* crearSegmentoCero();
  Segmento* crearSegmento(int id, int size);
  Segmento* buscarCandidato(int tamanio);
  t_list* buscarSegmentoSegunTamanio(int tamanio);
  void guardarEnMemoria(void* elemento, Segmento* segmento, int size);
  void ocuparMemoria(void* tareas, int base, int size);
  void ocuparBitMap(t_bitarray* bitMap, int base, int size);
  void ocuparBitArray(Segmento* segmento);
  int puedoGuardar(int quieroGuardar);
  t_list* obtenerContenidoSegmentos();
  int tamanioTotalDisponible(void);
  void liberarListaSegmentos(t_list* segmentos);
  void ocuparMemoriaSegmento(Segmento* segmento, int* base);
  void ocuparMemoriaProceso(tablaDeSegmento* segmentosProceso, int* base);
  void ocuparMemoriaPrincipal();
  void compactacion();
  int contarCantidadDe(int base, int numero);
  void obtenerContenidoPorProceso(int idProceso, tablaDeSegmento* contenidoProceso);
  void limpiarArrayBits();
  t_list* puedenGuardar(t_list* segmentos, int size);
  Segmento* elegirCriterio (t_list* segmentos, int tamanio);
  Segmento* segmentoBest(t_list* segmentos, int tamanio);
  Segmento* segmentoWorst(t_list* segmentos, int tamanio);
#endif
