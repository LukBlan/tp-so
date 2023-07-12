#include <utils.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void validarCantidadArgumentosMain(int cantidadArgumentos, int cantidadArgumenosEsperadosada) {
   if (cantidadArgumentos != cantidadArgumenosEsperadosada) {
     printf("Cantidad de argumentos incorrectos");
     exit(-1);
   }
 }
archivoAbierto* buscarNombre(t_list* archivos, char* nombreArch){
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
t_list* f_seek(char* nomArchivo, t_list* archivosAbiertos, int posicion){
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
  printf("Ax %s ", registros.AX);
  printf("Bx %s ", registros.BX);
  printf("Cx %s ", registros.CX);
  printf("Dx %s\n", registros.DX);
}

void mostrarContexto(contextoEjecucion* contexto) {
  puts("----------------------------------------");
  puts("         Contexto Ejecucion             ");
  puts("----------------------------------------");
  printf("programCounter = %d\n", contexto->programCounter);
  mostrarInstrucciones(contexto->instrucciones);
  mostrarRegistros(contexto->registros);
  printf("Cantidad Segmentos %d\n", contexto->tablaSegmentos->elements_count);
  printf("Cantidad de Archivos = %d\n", contexto->archivosAbiertos->elements_count);
  puts("----------------------------------------");
}

int bitsToBytes(int bits) {
	return (int) ceil(bits / 8.0);
}

char* generarArray(int bytes) {
  char* aux = malloc(bytes);
  memset(aux, 0, bytes); // SETEA LOS BYTES EN 0
  return aux;
}
