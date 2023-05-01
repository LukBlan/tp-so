/*
 * estructuras.h
 *
 *  Created on: Apr 18, 2023
 *      Author: utnso
 */

#ifndef INCLUDE_ESTRUCTURAS_H_
#define INCLUDE_ESTRUCTURAS_H_
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

typedef enum op_code{
	DESCONEXION = -1,
	MENSAJE,
	LINEAS_INSTRUCCION
}op_code;

typedef struct linea_Instruccion {
	char *identifier;
	int parametros[3];
}Linea_Instruccion;

typedef struct{
	int size;
	void *stream;
}t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} Paquete;

#endif /* INCLUDE_ESTRUCTURAS_H_ */
