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
  puts("----------------------------------------");
}

int bitsToBytes(int bits) {
	return (int) ceil(bits / 8.0);
}

char* generarArray(int bytes){
    char* aux = malloc(bytes);
    memset(aux, 0, bytes); // SETEA LOS BYTES EN 0
    return aux;
}
