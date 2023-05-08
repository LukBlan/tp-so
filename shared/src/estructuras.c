#include <estructuras.h>

void eliminar_paquete(Paquete *paquete){
	if (paquete != NULL){
		if(paquete-> buffer !=NULL){
			free(paquete->buffer->stream);
			free(paquete->buffer);
		}
		free(paquete);
	}
}

void iniciar_buffer(Paquete *paquete){
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

Paquete *crear_paquete (op_code codigoOperacion){
	Paquete *paquete = malloc(sizeof(Paquete));
	paquete->codigo_operacion = codigoOperacion;
	iniciar_buffer(paquete);
	return paquete ;
}
void agregar_a_paquete(Paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void* serializar_paquete(Paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void enviar_paquete(Paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void serializar_lista_instruc (Paquete *paquete, t_list *instrucciones){
	int cantDeInstrucciones  = list_size(instrucciones);
	agregar_a_paquete(paquete,&cantDeInstrucciones,sizeof(int));
	Linea_Instruccion *linea;
	//Mientras hay instrucciones se siguen sumando al paquete
	for(int k=0;k<cantDeInstrucciones;k++)
	{
		linea = list_get(instrucciones,k);
		agregar_a_paquete(paquete,linea->identifier,strlen(linea->identifier)+1);
		for(int i = 0;i<=2;i++){
		//agregar_a_paquete(paquete, &(linea->parametros[i]), sizeof(linea->parametros[i] + 1));
		}
	}
}
