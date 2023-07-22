#include <utils.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

void validarCantidadArgumentosMain(int cantidadArgumentos, int cantidadArgumenosEsperadosada) {
   if (cantidadArgumentos != cantidadArgumenosEsperadosada) {
     printf("Cantidad de argumentos incorrectos");
     exit(-1);
   }
 }

archivoAbierto* buscarNombre(t_list* archivos, char* nombreArch) {
    archivoAbierto* archivo;
    for (int i = 0; i < list_size(archivos); i++){
        archivoAbierto* archivoActual = list_get(archivos, i);
    if (strcmp(archivoActual->nombre, nombreArch) == 0) {
        archivo = archivoActual;
        break;
    }
}
    return archivo;
}

t_list* f_seek(char* nomArchivo, t_list* archivosAbiertos, int posicion) {
      archivoAbierto* arch = buscarNombre(archivosAbiertos, nomArchivo);
      fseek(arch->punteroArchivo, posicion, SEEK_SET);
      return archivosAbiertos;
  }

void mostrarInstrucciones(t_list* listaInstrucciones) {
  int numeroInstrucciones = list_size(listaInstrucciones);
  for (int i = 0; i < numeroInstrucciones; i++) {
    t_instruccion* instruccion = list_get(listaInstrucciones, i);
    int cantidadStrings = instruccion->cantidadParametros + 1;

    for (int i = 0; i < cantidadStrings; i++) {
      printf("%s ", instruccion->strings[i]);
    }
    puts("");
  }
}

void mostrarRegistros(t_registros registros) {
  puts("----------- Registros ------------------");
  printf("|Ax - %s||\n", registros.AX);
  printf("|Bx - %s||\n", registros.BX);
  printf("|Cx - %s||\n", registros.CX);

  printf("|Dx - %s||\n", registros.DX);
}

void mostrarSegmentos(t_list* segmentos) {
  int cantidadSegmentos = segmentos->elements_count;
  printf("Cantidad Segmentos %d\n", cantidadSegmentos);

  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(segmentos, i);
    printf("Segmento id: %d, base: %d, limite: %d\n", segmento->id, segmento->base, segmento->limite);
  }
}

void mostrarContexto(contextoEjecucion* contexto) {
  puts("----------------------------------------");
  puts("         Contexto Ejecucion             ");
  puts("----------------------------------------");
  printf("programCounter = %d\n", contexto->programCounter);
  mostrarInstrucciones(contexto->instrucciones);
  mostrarRegistros(contexto->registros);
  mostrarSegmentos(contexto->tablaSegmentos);
  printf("Cantidad de Archivos = %d\n", contexto->archivosAbiertos->elements_count);
  puts("----------------------------------------");
}

void mostrarTablaDeSegmentos(t_list* tablaDeSegmentosPorProceso) {
  int cantidadProcesos = tablaDeSegmentosPorProceso->elements_count;
  puts("--------------------- Tabla de Segmentos ------------------");
  printf("Cantidad de Procesos %d\n", cantidadProcesos);
  for (int i = 0; i < cantidadProcesos; i++) {
    tablaDeSegmento* tabla = list_get(tablaDeSegmentosPorProceso, i);
    printf("Proceso Id: %d\n", tabla->id);

    int cantidadSegmentos = tabla->segmentos_proceso->elements_count;
    printf("-------- Segmentos %d --------\n", cantidadSegmentos);
    for (int j = 0; j < cantidadSegmentos; j++) {
      Segmento* segmento = list_get(tabla->segmentos_proceso, j);
      printf("id: %d base: %d limite: %d\n", segmento->id, segmento->base, segmento->limite);
    }
  }
}

int bitsToBytes(int bits) {
	return (int) ceil(bits / 8.0);
}

char* generarArray(int bytes) {
  char* aux = malloc(bytes);
  memset(aux, 0, bytes); // SETEA LOS BYTES EN 0
  return aux;
}
