include <string.h>

void serializarEntero(t_buffer* buffer, int valorEntero) {
  memcpy(buffer->stream, &(valorEntero), sizeof(int));
}
