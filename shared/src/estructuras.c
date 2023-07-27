#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void liberarInstrucciones(t_list* instrucciones) {
  int numeroInstrucciones = list_size(instrucciones);

  for (int i = 0; i < numeroInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    int cantidadStrings = instruccion->cantidadParametros + 1;
    for (int i = 0; i < cantidadStrings; i++) {
      free(instruccion->strings[i]);
    }
    free(instruccion);
  }
  list_destroy(instrucciones);
}

void liberarTablaDeSegmentos(t_list* segmentos) {
  int cantidadSegmentos = segmentos->elements_count;
  for(int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(segmentos, i);
    free(segmento);
  }

  list_destroy(segmentos);
}

void liberarArchivosAbiertos(t_list* archivosAbiertos) {
  int cantidadArchivos = archivosAbiertos->elements_count;

    for(int i = 0; i < cantidadArchivos; i++) {
      free(list_get(archivosAbiertos, i));
    }

    list_destroy(archivosAbiertos);
}

void liberarContexto(contextoEjecucion* contexto) {
  liberarInstrucciones(contexto->instrucciones);
  liberarTablaDeSegmentos(contexto->tablaSegmentos);
  liberarArchivosAbiertos(contexto->archivosAbiertos);
  free(contexto);
}

void liberarPcb(PCB* pcb) {
  liberarContexto(pcb->contexto);
  free(pcb);
}
