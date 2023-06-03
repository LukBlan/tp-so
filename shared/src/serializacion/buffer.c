#include <serializacion/buffer.h>
#include <stdlib.h>

int tamanioBuferInstrucciones(t_list* instrucciones) {
  int cantidadInstrucciones = list_size(instrucciones);
  int tamanioTotal = sizeof(int);

  for (int i = 0; i < cantidadInstrucciones; i++) {
    t_instruccion* instruccion = list_get(instrucciones, i);
    tamanioTotal += instruccion->longitudIdentificador;
    tamanioTotal += instruccion->longitudParametros[0];
    tamanioTotal += instruccion->longitudParametros[1];
    tamanioTotal += instruccion->longitudParametros[2];
    tamanioTotal += sizeof(int) * 4;
  }
  return tamanioTotal;
}

t_buffer* generarBuffer(int tamanio) {
  t_buffer* buffer = malloc(tamanio + sizeof(int));
  buffer->stream = malloc(tamanio);;
  buffer->size = tamanio;
  return buffer;
}

void iniciar_buffer(t_paquete* paquete){
  t_buffer* buffer = NULL;
  paquete->buffer->size = 0;
  paquete->buffer = malloc(sizeof(buffer));
}

