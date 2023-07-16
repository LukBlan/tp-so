#include <recursos.h>
#include <segmentacion.h>
#include <stdlib.h>
#include <utils.h>
#include <segmentacion.h>
#include <string.h>

Segmento* segmentoCero;
char* arrayDeHuecos;
t_bitarray* bitMapSegmento;
t_list* tablaDeSegmentosPorProceso;

void iniciarSegmentacion() {
    int tamanio = recursosMemoria->configuracion->TAM_MEMORIA;
    arrayDeHuecos = generarArray(recursosMemoria->configuracion->TAM_MEMORIA);
    bitMapSegmento = bitarray_create_with_mode(arrayDeHuecos,tamanio, MSB_FIRST);
}

Segmento* crearSegmentoCero() {
  Segmento* segmentoCero = malloc(sizeof(Segmento));
  int tamanioSegmentoCero = recursosMemoria->configuracion->TAM_SEGMENTO_0;

  segmentoCero->id = 1;
  segmentoCero->base = 0;
  segmentoCero->limite = tamanioSegmentoCero;

  ocuparBitArray(segmentoCero);
  return segmentoCero;
}

void ocuparBitArray(Segmento* segmento) {
  int base = segmento->base;
  int tamanio = segmento->limite;

  for (int i = base * 8; i < (base + tamanio) * 8; i++) {
    bitarray_set_bit(bitMapSegmento, i);
  }
}

// DEVUELVE EL SEGMENTO QUE FUE GUARDADO
Segmento* crearSegmento(int id, int size) {
  Segmento* segmentoNuevo = buscarCandidato(size);

  segmentoNuevo->id = id;
  segmentoNuevo->limite = size;
  ocuparBitArray(segmentoNuevo);

  return segmentoNuevo;
}

void liberarListaSegmentos(t_list* segmentos) {
  int cantidadSegmentos = segmentos->elements_count;

  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmentoNuevo = list_get(segmentos, i);
    free(segmentoNuevo);
  }

  list_destroy(segmentos);
}

void ocuparMemoriaSegmento(Segmento* segmento, contenidoSegmento* contenidoEnSegmento, int* base) {
  memcpy(memoriaPrincipal, contenidoEnSegmento->contenido, segmento->limite);
  *base += segmento->limite;
  segmento->base = *base;
  ocuparBitArray(segmento);
}

void ocuparMemoriaProceso(tablaDeSegmento* contenidoProceso, tablaDeSegmento* segmentosProceso, int* base) {
  int cantidadSegmentos = contenidoProceso->segmentos_proceso->elements_count;
  t_list* listaContenido = contenidoProceso->segmentos_proceso;
  t_list* listaSegmentos = segmentosProceso->segmentos_proceso;

  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(listaSegmentos, i);
    contenidoSegmento* contenido = list_get(listaContenido, i);
    ocuparMemoriaSegmento(segmento, contenido, &base);
  }
}

void ocuparMemoriaPrincipal(t_list* contenidoSegmentosPorProceso) {
  int cantidadProcesos = contenidoSegmentosPorProceso->elements_count;
  int base = recursosMemoria->configuracion->TAM_SEGMENTO_0;

  for (int i = 0; i < cantidadProcesos; i++) {
    tablaDeSegmento* contenidoProceso = list_get(contenidoSegmentosPorProceso, i);
    tablaDeSegmento* segmentosProceso = list_get(tablaDeSegmentosPorProceso, i);
    ocuparMemoriaProceso(contenidoProceso, segmentosProceso, &base);
  }
}

void limpiarArrayBits() {
  int tamanioMemoria = recursosMemoria->configuracion->TAM_MEMORIA * 8;

  for(int i = 0; i < tamanioMemoria; i++) {
    bitarray_clean_bit(bitMapSegmento, i);
  }
}

void obtenerContenidoPorProceso(int idProceso, tablaDeSegmento* contenidoProceso) {
  tablaDeSegmento* segmentosProceso = list_get(tablaDeSegmentosPorProceso, idProceso);
  contenidoProceso->segmentos_proceso = list_create();
  t_list* listaSegmentos = segmentosProceso->segmentos_proceso;
  int cantidadSegmentos = listaSegmentos->elements_count;
  contenidoProceso->id = idProceso;

  for (int i = 0; cantidadSegmentos; i++) {
    contenidoSegmento* contenido = malloc(sizeof(contenidoSegmento));
    Segmento* segmento = list_get(listaSegmentos, i);
    void* contenidoMemoria = malloc(segmento->limite);
    memcpy(contenidoMemoria, memoriaPrincipal + segmento->base, segmento->limite);

    contenido->idSegmento = segmento->id;
    contenido->contenido = contenidoMemoria;
    list_add(contenidoProceso->segmentos_proceso, contenido);
  }
}

t_list* obtenerContenidoSegmentos() {
  int cantidadProcesos = tablaDeSegmentosPorProceso->elements_count;
  t_list* contenidoSegmentos = list_create() ;
  for (int i = 0; i < cantidadProcesos; i++) {
    tablaDeSegmento* contenidoProceso = malloc(sizeof(tablaDeSegmento));
    obtenerContenidoPorProceso(i, contenidoProceso);
    list_add(contenidoSegmentos, contenidoProceso);
  }
  return contenidoSegmentos;
}

void compactacion() {
  t_list* contenidoDeSegmentoPorProceso = obtenerContenidoSegmentos();
  limpiarArrayBits();
  ocuparMemoriaPrincipal(contenidoDeSegmentoPorProceso);
}

Segmento* buscarCandidato(int tamanio) {
  // Genero un segmento auxiliar para guardar la info antes de liberar listas
  Segmento* segmentoEncontrado;
  Segmento* segmentoNuevo = malloc(sizeof(Segmento));
  t_list* todosLosSegLibres;

  todosLosSegLibres = buscarSegmentoSegunTamanio(tamanio);

  if(list_is_empty(todosLosSegLibres)) {
    puts("Estamos Compactando ......");
    //compactacion();
    //segmento = buscarCandidato(tamanio);
  } else if (list_size(todosLosSegLibres) == 1) {
    segmentoEncontrado = list_get(todosLosSegLibres, 0);
  } else {
    segmentoEncontrado = elegirCriterio(todosLosSegLibres, tamanio);
  }

  segmentoNuevo->base = segmentoEncontrado->base;

  liberarListaSegmentos(todosLosSegLibres);
  return segmentoNuevo;
}

int contarCantidadDe(int base, int numero) {
  int tamanioMemoria = recursosMemoria->configuracion->TAM_MEMORIA;
  int cantidad = 0;

  while((base + cantidad < tamanioMemoria * 8) && bitarray_test_bit(bitMapSegmento, base + cantidad) == numero) {
    cantidad++;
  }
  return cantidad;
}

t_list* buscarSegmentoSegunTamanio(int tamanioMinimo) {
  t_list* segmentosDisponibles = list_create();
  int base = 0;
  int tamanio = 0 ;

  //mutex a bitarray
  while((base / 8) < (recursosMemoria->configuracion->TAM_MEMORIA)) {
    Segmento* unSegmento = malloc(sizeof(Segmento));

    int desplazamiento = contarCantidadDe(base, 1);
    base += desplazamiento;
    tamanio = contarCantidadDe(base, 0);
    unSegmento->base = base / 8;
    unSegmento->limite = tamanio / 8;

    base += tamanio;

    if ((tamanio / 8) >= tamanioMinimo) {
      printf("Busco segmento de tamanio %d\n", tamanioMinimo);
      printf("Encontre segmento base %d tamanio %d\n", unSegmento->base, unSegmento->limite);
      list_add(segmentosDisponibles, unSegmento);
    } else {
      free(unSegmento);
    }
  }

  return segmentosDisponibles;
}

void ocuparMemoria(void* tareas, int base, int size) {
	//pthread_mutex_lock(&mutexMemoria);
  memcpy(memoriaPrincipal+base, tareas, size);
  //pthread_mutex_unlock(&mutexMemoria);
}

int puedoGuardar(int quieroGuardar) {
    int tamanioLibre = tamanioTotalDisponible();
    printf("Hay disponible %d de memoria", tamanioLibre);
    return (quieroGuardar <= tamanioLibre)? 1 : 0;
}

int tamanioTotalDisponible(void) {
    int cantidadDisponible = 0;
    int desplazamiento = 0 ;

    while (desplazamiento < recursosMemoria->configuracion->TAM_MEMORIA * 8) {
    	//pthread_mutex_lock(&mutexBitMapSegment);
      if((bitarray_test_bit(bitMapSegmento, desplazamiento) == 0)) {
        cantidadDisponible ++;
      }
      //pthread_mutex_unlock(&mutexBitMapSegment);
      desplazamiento ++;
    }
    return cantidadDisponible / 8;
}

Segmento* elegirCriterio (t_list* segmentos, int tamanio) {
  Segmento* segmento;
  char*  algoritmo = recursosMemoria->configuracion->ALGOTIRMO_ASIGNACION;

  if(strcmp(algoritmo, "FIRST") == 0) {
    segmento = list_get(segmentos, 0);
  } else if (strcmp(algoritmo, "BEST") == 0) {
    //segmento = segmentoBest(segmentos, tamanio);
  } else if (strcmp(algoritmo, "WORST") == 0) {
    //segmento = segmentoWorst(segmentos, tamanio);
  }
  return segmento;
}
