#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_


typedef struct {
	char* PUERTO_ESCUCHA;
	int TAM_MEMORIA;
	int TAM_SEGMENTO_0;
	int CANT_SEGMENTOS;
	int RETARDO_MEMORIA;
	int RETARDO_COMPACTACION;
	char* ALGOTIRMO_ASIGNACION;
} t_configuracion;

typedef struct {
  t_log* logger;
  t_configuracion configuracion;
  int socketServidor;
};

t_configuracion* obtenerConfiguracion(char* pathConfiguracion);

#endif /* CONFIGURACION_H_ */
