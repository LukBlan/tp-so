#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_


typedef struct {
	int RETARDO_INSTRUCCION;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	int PUERTO_ESCUCHA;
	int TAM_MAX_SEGMENTO;
}configuracion;

configuracion obtenerConfiguracion();

#endif /* CONFIGURACION_H_ */
