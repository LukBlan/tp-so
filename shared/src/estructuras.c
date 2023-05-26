#include <estructuras.h>

op_code obtener_codigo_operacion(int socketCliente) {
  op_code codigoOperacion;
  if (recv(socketCliente, &codigoOperacion, sizeof(int), MSG_WAITALL) > 0) {
    printf("codigo operacion %d ", codigoOperacion);
    return codigoOperacion;
  } else {
    puts("DESCONEXION");
    close(socketCliente);
    return DESCONEXION;
  }
}

void eliminar_paquete(paquete* paquete){
	if (paquete != NULL){
		if(paquete-> buffer !=NULL){
			free(paquete->buffer->stream);
			free(paquete->buffer);
		}
		free(paquete);
	}
}

void iniciar_buffer(paquete* paquete){
	paquete->buffer = malloc(sizeof(buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

paquete* crear_paquete (op_code codigoOperacion){
	paquete* paquete = malloc(sizeof(paquete));
	paquete->codigo_operacion = codigoOperacion;
	iniciar_buffer(paquete);
	return paquete ;
}

void agregar_a_paquete(paquete* paquete, void* valor, int tamanio) {
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);
	paquete->buffer->size += tamanio + sizeof(int);
}

void* serializar_paquete(paquete* paquete, int bytes) {
	void* magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void enviar_paquete(paquete* paquete, int socketCliente) {
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);
	send(socketCliente, a_enviar, bytes, 0);
	free(a_enviar);
}

void serializarInstrucciones(paquete* paquete, t_list *instrucciones) {
	int cantDeInstrucciones = list_size(instrucciones);
	instruccion *linea;

	agregar_a_paquete(paquete, &cantDeInstrucciones, sizeof(int));
	//Mientras hay instrucciones se siguen sumando al paquete
	for(int k=0; k < cantDeInstrucciones; k++) {
		linea = list_get(instrucciones, k);
		agregar_a_paquete(paquete, linea->identificador, strlen(linea->identificador) + 1);
		//agregar_a_paquete(paquete, &(linea->parametros[0]), sizeof(union Parametro));
		//agregar_a_paquete(paquete, &(linea->parametros[1]), sizeof(union Parametro));
		//agregar_a_paquete(paquete, &(linea->parametros[2]), sizeof(union Parametro));
	}
}
