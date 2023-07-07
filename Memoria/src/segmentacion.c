#include <recursos.h>
#include <segmentacion.h>
#include <stdlib.h>
#include <utils.h>
#include <segmentacion.h>
#include <string.h>

Segmento* segmentoCero;
char* arrayDeHuecos;
t_bitarray* bitMapSegmento;

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

  for (int i = base; i < base + tamanio; i++) {
    bitarray_set_bit(bitMapSegmento, i);
  }
}

// DEVUELVE EL SEGMENTO QUE FUE GUARDADO
Segmento* crearSegmento(int id, int size) {
  Segmento* segmentoNuevo = buscarCandidato(size);

  segmentoNuevo->id = id;
  segmentoNuevo->limite= size;
  ocuparBitArray(segmentoNuevo);

  return segmentoNuevo;
}

Segmento* buscarCandidato(int tamanio) {
  Segmento* segmento;
  t_list* todosLosSegLibres;
  t_list* segmentosCandidatos;

  todosLosSegLibres = buscarSegmentosDisponibles();
  segmentosCandidatos = puedenGuardar(todosLosSegLibres , tamanio);

  if(list_is_empty(segmentosCandidatos)) {
    puts("Compactando... (En verdad no estamos haciendo nada, pero bueno yo q c)");
    //compactacion();
    //segmento = buscarCandidato(tamanio);
  } else if (list_size(segmentosCandidatos) == 1) {
    segmento = list_get(segmentosCandidatos, 0);
  } else {
    segmento = elegirCriterio(segmentosCandidatos, tamanio);
  }

  //TODO list destroy
  return segmento;
}

int contarCantidadDe(int base, int numero) {
  int cantidad = 0;

  while(bitarray_test_bit(bitMapSegmento, base + cantidad) == numero) {
    cantidad++;
  }
  return cantidad;
}

t_list* buscarSegmentosDisponibles() {
  t_list* segmentosDisponibles = list_create();
  int base = 0;
  int tamanio = 0 ;

  //mutex a bitarray
  while(base < (recursosMemoria->configuracion->TAM_MEMORIA)) {
    Segmento* unSegmento = malloc(sizeof(Segmento));

    int desplazamiento = contarCantidadDe(base, 1);
    base += desplazamiento;

    tamanio = contarCantidadDe(base, 0);
    unSegmento->base = base;
    unSegmento->limite = tamanio;
    base += tamanio;

    list_add(segmentosDisponibles, unSegmento);
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
    int contador = 0;
    int desplazamiento = 0 ;

    while (desplazamiento < recursosMemoria->configuracion->TAM_MEMORIA) {
    	//pthread_mutex_lock(&mutexBitMapSegment);
      if((bitarray_test_bit(bitMapSegmento, desplazamiento) == 0)) {
        contador ++;
      }
      //pthread_mutex_unlock(&mutexBitMapSegment);
      desplazamiento ++;
    }
    return contador;
}

t_list* puedenGuardar(t_list* segmentos, int size) {
    t_list* segmentosTamanioNecesario;

    int puedoGuardarSeg(Segmento* segmento) {
        return(segmento->limite >= size);
    }

    segmentosTamanioNecesario = list_filter(segmentos, (void*)puedoGuardarSeg);
    return segmentosTamanioNecesario;
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
