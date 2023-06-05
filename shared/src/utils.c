#include <utils.h>
#include <stdlib.h>

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
    for (int i = 0; i < list_size(instruccion->strings); i++) {
      printf("%s ", list_get(instruccion->strings, i), list_get(instruccion->sizeStrings, i));
    }
    puts("");
  }
}

