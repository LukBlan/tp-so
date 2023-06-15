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
    int cantidadStrings = instruccion->cantidadParametros + 1;

    for (int i = 0; i < cantidadStrings; i++) {
      printf("%s ", instruccion->strings[i]);
    }
    puts("");
  }
}

void mosrarRegistros(t_registros registros) {

}

void mostrarContexto(contextoEjecucion* contexto) {
  puts("----------------------------------------");
  puts("         Contexto Ejecucion             ");
  puts("----------------------------------------");
  printf("programCounter = %d\n", contexto->programCounter);
  mostrarInstrucciones(contexto->instrucciones);
  mosrarRegistros(contexto->registros);
  puts("----------------------------------------");
}
