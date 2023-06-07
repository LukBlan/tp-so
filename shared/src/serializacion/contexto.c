#include <serializacion/contexto.h>

int tamanioBytesContexto(contextoEjecucion* contexto) {
  int tamanioTotal = 0;
  tamanioTotal += tamanioBytesInstruccion(contexto->instrucciones);
  tamanioTotal += sizeof(int) * 3;
  tamanioTotal += sizeof(float);
  return tamanioTotal;
}
