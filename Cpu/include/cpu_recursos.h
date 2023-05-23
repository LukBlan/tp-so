#include <commons/log.h>

#ifndef CPU_RECURSOS_H_
#define CPU_RECURSOS_H_

typedef struct {
	int RETARDO_INSTRUCCION;
	char* IP_MEMORIA;
	char* IP_ESCUCHA;
	int PUERTO_MEMORIA;
	int PUERTO_ESCUCHA;
	int TAM_MAX_SEGMENTO;
} configuracion;

typedef struct {
	configuracion* configuracion;
	t_log* logger;
} recursos;

configuracion* obtenerConfiguracion(char*);
recursos* obtenerRecursosModulo(char*, char*);
void liberarRecursos(recursos*);

#endif /* CPU_RECURSOS_H_ */
