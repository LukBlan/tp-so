#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_


typedef struct {
	int PUERTO_ESCUCHA;
	int TAM_MEMORIA;
	int TAM_SEGMENTO_0;
	int CANT_SEGMENTOS;
	int RETARDO_MEMORIA;
	int RETARDO_COMPACTACION;
	char* ALGOTIRMO_ASIGNACION;
}configuracion;

configuracion obtenerConfiguracion();

#endif /* CONFIGURACION_H_ */
