#include <utils.h>
#include <stdlib.h>

void validarCantidadArgumentosMain(int cantidadArgumentos, int cantidadArgumenosEsperadosada) {
   if (cantidadArgumentos != cantidadArgumenosEsperadosada) {
     printf("Cantidad de argumentos incorrectos");
     exit(-1);
   }
 }

void mostrarInstrucciones(t_list* instrucciones) {
  int tamaño = list_size(instrucciones);
  for (int i = 0; i < tamaño; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    printf("%s", instruccion->identificador);
    printf(" %s", instruccion->parametros[0]);
    printf(" %s", instruccion->parametros[1]);
    printf(" %s\n", instruccion->parametros[2]);
  }
}
