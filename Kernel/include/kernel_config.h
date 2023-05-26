#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

typedef struct {
	char* IP_ESCUCHA;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	char* IP_FILESYSTEM;
	int PUERTO_FILESYSTEM;
	char* IP_CPU;
	int PUERTO_CPU;
	int PUERTO_ESCUCHA;
	char* ALGORITMO_PLANIFICACION;
	int ESTIMACION_INICIAL;
	int HRRN_ALFA;
	int GRADO_MAX_MULTIPROGRAMACION;
	int RECURSOS;//es una lista de char
	int INSTANCIAS_RECURSOS;// es una lista de int
} configuracion;

configuracion* obtenerConfiguracion();

#endif /* KERNEL_CONFIG_H_ */