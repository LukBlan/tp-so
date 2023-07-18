#include <serializacion/tablaDeSegmentos.h>
#include <serializacion/contexto.h>

void serializarTablaDeSegmentos(t_buffer* buffer, t_list* tablaDeSegmentos) {
}

int tamanioBytesTablaDeSegmentos(t_list* tablaDeSegmentos) {
  int cantidadDeProceso = tablaDeSegmentos->elements_count;
  int tamanioTabla = sizeof(int); // cantidad de procesos

  for (int i = 0; i < cantidadDeProceso; i++) {
    tablaDeSegmento* segmentosProceso = list_get(tablaDeSegmentos, i);
    tamanioTabla += sizeof(int); // id proceso
    tamanioTabla += tamanioBytesSegmentos(segmentosProceso->segmentos_proceso);
  }
  return tamanioTabla;
}
