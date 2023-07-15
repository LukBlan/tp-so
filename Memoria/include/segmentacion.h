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
  int puedoGuardar(int quieroGuardar);
  int tamanioTotalDisponible(void);
  t_list* puedenGuardar(t_list* segmentos, int size);
  Segmento* elegirCriterio (t_list* segmentos, int tamanio);

#endif
