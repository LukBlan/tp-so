#ifndef CPU_CONFIG_H_
#define CPU_CONFIG_H_


typedef struct {
	int RETARDO_INSTRUCCION;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	int PUERTO_ESCUCHA;
	int TAM_MAX_SEGMENTO;
}configuracion;

configuracion obtenerConfiguracion();

#endif /* CPU_CONFIG_H_ */
