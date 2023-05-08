#ifndef INCLUDE_ESTRUCTURAS_H_
#define INCLUDE_ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/collections/list.h>

typedef enum op_code{
	DESCONEXION = -1,
	LINEAS_INSTRUCCION,
	PCB
}op_code;

typedef enum TipoInstruccion {
	DESCONOCIDA = -1,
	SET,
	YIELD,
	EXIT
}TipoInstruccion;

union Parametro{
	int integer;
	char* string;
};

typedef struct Linea_Instruccion {
	char *identifier;
	union Parametro parametros[3];
}Linea_Instruccion;

typedef struct{
	int size;
	void *stream;
}t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} Paquete;

void eliminar_paquete(Paquete *paquete);
void iniciar_buffer(Paquete *paquete);
Paquete *crear_paquete (op_code codigoOperacion);
void agregar_a_paquete(Paquete* paquete, void* valor, int tamanio);
void* serializar_paquete(Paquete* paquete, int bytes);
void enviar_paquete(Paquete* paquete, int socket_cliente);
#endif /* INCLUDE_ESTRUCTURAS_H_ */
