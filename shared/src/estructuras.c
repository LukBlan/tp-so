#include <estructuras.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

op_code obtener_codigo_operacion(int socketCliente) {
  op_code codigoOperacion;
  if (recv(socketCliente, &codigoOperacion, sizeof(int), MSG_WAITALL) > 0) {
    return codigoOperacion;
  } else {
    puts("DESCONEXION");
    close(socketCliente);
    return DESCONEXION;
  }
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

void iniciar_buffer(t_paquete* paquete){
  t_buffer* buffer = NULL;
  paquete->buffer->size = 0;
	paquete->buffer = malloc(sizeof(buffer));
}

t_paquete* crear_paquete(op_code codigoOperacion){
	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->codigo_operacion = codigoOperacion;
	iniciar_buffer(paquete);
	return paquete ;
}

void serializarInstrucciones(t_paquete* paquete, t_list *instrucciones) {
  int cantDeInstrucciones = list_size(instrucciones);

  agregar_a_paquete(paquete, &cantDeInstrucciones, sizeof(int));
  //Mientras hay instrucciones se siguen sumando al paquete
  for(int i = 0; i < cantDeInstrucciones; i++) {
    t_instruccion *linea = list_get(instrucciones, i);
    agregar_a_paquete(paquete, linea->identificador, linea->longitudIdentificador);
    agregar_a_paquete(paquete, &(linea->parametros[0]), linea->longitudParametros[0]);
    agregar_a_paquete(paquete, &(linea->parametros[1]), linea->longitudParametros[1]);
    agregar_a_paquete(paquete, &(linea->parametros[2]), linea->longitudParametros[2]);
  }
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio) {
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);
	paquete->buffer->size += tamanio + sizeof(int);
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

void enviar_paquete(t_paquete* paquete, int socketCliente) {
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);
	send(socketCliente, a_enviar, bytes, 0);
	free(a_enviar);
}
/*
char *obtener_mensaje_del_servidor(int socketServidor)
{
  t_list* listaMensaje;
  char* mensaje;

  switch (obtener_codigo_operacion(socketServidor))
  {
  case MENSAJE:
    listaMensaje = obtenerLista(socketServidor);
    mensaje = string_duplicate((char *)list_get(listaMensaje, 0));
    list_destroy_and_destroy_elements(listaMensaje, &free);
    break;

  default:
    break;
  }

  return mensaje;
}
t_list* obtenerLista(int socketCliente)
{
  int tamanioBuffer;
  int tamanioContenido;
  int desplazamiento = 0;

  t_list *contenido = list_create();
  void *buffer = obtenerBuffer(&tamanioBuffer, socketCliente);

  while (desplazamiento < tamanioBuffer)
  {
    memcpy(&tamanioContenido, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    void *valor = malloc(tamanioContenido);
    memcpy(valor, buffer + desplazamiento, tamanioContenido);
    desplazamiento += tamanioContenido;

    list_add(contenido, valor);
  }

  free(buffer);
  return contenido;
}
void *obtenerBuffer(int *tamanio, int socketCliente)
{
  void *buffer;

  recv(socketCliente, tamanio, sizeof(int), MSG_WAITALL);
  buffer = malloc(*tamanio);
  recv(socketCliente, buffer, *tamanio, MSG_WAITALL);

  return buffer;
}*/