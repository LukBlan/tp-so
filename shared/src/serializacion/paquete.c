#include <serializacion/paquete.h>
#include <serializacion/buffer.h>
#include <stdlib.h>
#include <string.h>

t_paquete* generarPaquete(t_buffer* buffer, op_code codigoOperacion) {
  t_paquete* paquete = malloc(buffer->size + sizeof(int) * 2);
  paquete->buffer = buffer;
  paquete->codigo_operacion = codigoOperacion;
  return paquete;
}

void liberarPaquete(t_paquete* paquete) {
  free(paquete->buffer->stream);
  free(paquete->buffer);
  free(paquete);
}

t_paquete* crear_paquete(op_code codigoOperacion){
	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->codigo_operacion = codigoOperacion;
	iniciar_buffer(paquete);
	return paquete ;
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

void eliminar_paquete(t_paquete* paquete){
	if (paquete != NULL){
		if(paquete->buffer != NULL){
			free(paquete->buffer->stream);
			free(paquete->buffer);
		}
		free(paquete);
	}
}
