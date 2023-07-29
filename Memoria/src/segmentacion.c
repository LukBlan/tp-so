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
op_code respuestaMemoria;
int clienteActual;

void iniciarSegmentacion() {
    int tamanio = recursosMemoria->configuracion->TAM_MEMORIA;
    arrayDeHuecos = generarArray(recursosMemoria->configuracion->TAM_MEMORIA);
    bitMapSegmento = bitarray_create_with_mode(arrayDeHuecos,tamanio, MSB_FIRST);
}

Segmento* crearSegmentoCero() {
  Segmento* segmentoCero = malloc(sizeof(Segmento));
  int tamanioSegmentoCero = recursosMemoria->configuracion->TAM_SEGMENTO_0;

  segmentoCero->id = 0;
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

void ocuparMemoriaSegmento(Segmento* segmento, contenidoSegmento* contenidoSegmento, int* base, int idProceso) {
  segmento->base = *base;
  memcpy(memoriaPrincipal + segmento->base, contenidoSegmento->contenido, segmento->limite);
  *base += segmento->limite;
  log_info(
    recursosMemoria->logger,
    "PID: <%d> - Segmento: <%d> - Base: <%d> - Tamaño <%d>",
    idProceso, segmento->id, segmento->base, segmento->limite
  );
  ocuparBitArray(segmento);
}

void ocuparMemoriaProceso(tablaDeSegmento* contenidoProceso, tablaDeSegmento* segmentosProceso, int* base) {
  int cantidadSegmentos = segmentosProceso->segmentos_proceso->elements_count;
  t_list* listaContenido = contenidoProceso->segmentos_proceso;
  t_list* listaSegmentos = segmentosProceso->segmentos_proceso;

  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(listaSegmentos, i);
    contenidoSegmento* contenido = list_get(listaContenido, i);
    ocuparMemoriaSegmento(segmento, contenido, base, segmentosProceso->id);
  }
}

void ocuparMemoriaPrincipal(t_list* listaDeProcesosConContenido) {
  int cantidadProcesos = tablaDeSegmentosPorProceso->elements_count;
  int base = recursosMemoria->configuracion->TAM_SEGMENTO_0;
  ocuparBitArray(segmentoCero);

  for (int i = 0; i < cantidadProcesos; i++) {
    tablaDeSegmento* contenidoProceso = list_get(listaDeProcesosConContenido, i);
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

  for (int i = 0; i < cantidadSegmentos; i++) {
    contenidoSegmento* contenido = malloc(sizeof(contenidoSegmento));
    Segmento* segmento = list_get(listaSegmentos, i);
    char* contenidoSring = malloc(5);
    void* contenidoMemoria = malloc(segmento->limite);
    memcpy(contenidoMemoria, memoriaPrincipal + segmento->base, segmento->limite);
    memcpy(contenidoSring, contenidoMemoria, 4);
    contenidoSring[4] = '\0';
    printf("El contenido es %s\n", contenidoSring);
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

contextoEjecucion* generarContextoAlPedo() {
  contextoEjecucion* nuevoContexto = malloc(sizeof(contextoEjecucion));
  nuevoContexto->archivosAbiertos = list_create();
  nuevoContexto->instrucciones = list_create();
  nuevoContexto->tablaSegmentos = list_create();
  return nuevoContexto;
}

void compactacion() {
  contextoEjecucion* contextoAlRePedo = generarContextoAlPedo();
  enviarContexto(contextoAlRePedo, clienteActual, COMPACTACION);
  int retardoCompactacion = recursosMemoria->configuracion->RETARDO_COMPACTACION * 1000;
  //usleep(retardoCompactacion);
  t_list* listaDeProcesosConContenido = obtenerContenidoSegmentos();
  limpiarArrayBits();
  ocuparMemoriaPrincipal(listaDeProcesosConContenido);
  respuestaMemoria = COMPACTACION;
}

Segmento* buscarCandidato(int tamanio) {
  // Genero un segmento auxiliar para guardar la info antes de liberar listas
  Segmento* segmentoEncontrado;
  Segmento* segmentoNuevo = malloc(sizeof(Segmento));
  t_list* todosLosSegLibres;

  todosLosSegLibres = buscarSegmentoSegunTamanio(tamanio);

  if(list_is_empty(todosLosSegLibres)) {
    log_info(recursosMemoria->logger, "Solicitud de Compactación");
    compactacion();
    segmentoEncontrado = buscarCandidato(tamanio);
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
    segmento = segmentoBest(segmentos, tamanio);
  } else if (strcmp(algoritmo, "WORST") == 0) {
    segmento = segmentoWorst(segmentos, tamanio);
  }
  return segmento;
}

Segmento* segmentoBest(t_list* segmentos, int tamanio) {
	int bestFitIndex = -1;
	int bestFitSize = -1;

	for(int i = 0; i < segmentos->elements_count; i++) {
	  Segmento* segmento = list_get(segmentos,i);

    if (bestFitSize == -1 || segmento->limite < bestFitSize) {
      bestFitSize = segmento->limite;
      bestFitIndex = i;
    }
	}

	Segmento* segmentoElejido = list_get(segmentos, bestFitIndex);
	return segmentoElejido;
}


Segmento* segmentoWorst(t_list* segmentos, int tamanio) {
  int worstFitSize = -1;
	int worstFitIndex = -1;

	for (int i = 0; i < segmentos->elements_count; i++) {
	  Segmento* unSegmento = list_get(segmentos, i);

    if (worstFitSize == -1 || unSegmento->limite > worstFitSize) {
      worstFitSize = unSegmento->limite;
      worstFitIndex = i;
    }
	}

	Segmento* segmentoElejido = list_get(segmentos, worstFitIndex);
	return segmentoElejido;
}
