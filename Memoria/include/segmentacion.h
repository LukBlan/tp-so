#ifndef SEGMENTACION_H
#define SEGMENTACION_H

  #include <recursos.h>
  #include <stdlib.h>
  #include <utils.h>
  #include <estructuras.h>
  #include <commons/bitarray.h>

  extern void * memoriaPrincipal;

  typedef struct {
    int id;
    t_list segmentos_proceso ;
  } tablaDeSegmento;

  typedef struct {
    int id;
    int base;
    int limite;
  } Segmento;

  extern char* tablaDeHuecos;
  extern t_bitarray* bitMapSegmento;
  extern Segmento* segmentoCero;
  extern char* arrayDeHuecos;

  void iniciarSegmentacion ();
  Segmento* crearSegmentoCero();
  Segmento* crearSegmento(void* elemento, int size,int id);
  void guardarEnMemoria(void* elemento, Segmento* segmento, int size);
  void ocuparMemoria(void* tareas, int base, int size);
  void ocuparBitMap(t_bitarray* bitMap, int base, int size);
  int puedoGuardar(int quieroGuardar);
  int tamanioTotalDisponible(void);
  t_list* buscarSegmentosDisponibles();
  t_list* puedenGuardar(t_list* segmentos, int size);
  Segmento* buscarCandidato(int tamanio);
  Segmento* elegirCriterio (t_list* segmentos, int tamanio);

#endif
