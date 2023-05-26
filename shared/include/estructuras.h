#ifndef INCLUDE_ESTRUCTURAS_H_
#define INCLUDE_ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/collections/list.h>

typedef struct {
	unsigned int pid;
	t_list *instrucciones;
	unsigned int programCounter;
	struct registros;
	struct tablaSegmentos;
	float estimadoRafaga;
	unsigned int llegadaReady;
	t_list *archivosAbiertos;
} PCB;

typedef enum {
	DESCONEXION = -1,
	LINEAS_INSTRUCCION,
	Pcb
} op_code;

typedef enum {
	DESCONOCIDA = -1,
	SET,
	YIELD,
	EXIT
} TipoInstruccion;

typedef struct {
	char *identificador;
	int longitudIdentificador;
	char* parametros[3];
	int longitudParametros[3];
} instruccion;

typedef struct {
	int size;
	void *stream;
} buffer;

typedef struct {
	op_code codigo_operacion;
	buffer* buffer;
} paquete;

void eliminar_paquete(paquete* paquete);
void iniciar_buffer(paquete* paquete);
paquete *crear_paquete (op_code codigoOperacion);
void agregar_a_paquete(paquete* paquete, void* valor, int tamanio);
void* serializar_paquete(paquete* paquete, int bytes);
void enviar_paquete(paquete* paquete, int socketCliente);
op_code obtener_codigo_operacion(int socketCliente);

#endif /* INCLUDE_ESTRUCTURAS_H_ */
