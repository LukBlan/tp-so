#include <serializacion/buffer.h>
#include <stdlib.h>

t_buffer* generarBuffer(int tamanio) {
  t_buffer* buffer = malloc(tamanio + sizeof(int));
  buffer->stream = malloc(tamanio);;
  buffer->size = tamanio;
  return buffer;
}

void liberarBuffer(t_buffer* buffer) {
  free(buffer->stream);
  free(buffer);
}
