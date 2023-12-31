#include <serializacion/paquete.h>
#include <serializacion/buffer.h>
#include <stdlib.h>
#include <string.h>

t_paquete* crearPaquete(t_buffer* buffer, op_code codigoOperacion) {
  t_paquete* paquete = malloc(sizeof(t_paquete));
  paquete->buffer = buffer;
  paquete->codigo_operacion = codigoOperacion;
  return paquete;
}

void liberarPaquete(t_paquete* paquete) {
  free(paquete->buffer->stream);
  free(paquete->buffer);
  free(paquete);
}

void* serializar_paquete(t_paquete* paquete, int bytes) {
	void* magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;
	return magic;
}
