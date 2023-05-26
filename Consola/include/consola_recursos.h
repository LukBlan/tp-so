#ifndef CONSOLA_RECURSOS_H
#define CONSOLA_RECURSOS_H

#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>

typedef struct {
  char *IP_KERNEL;
  char *PUERTO_KERNEL;
} configuracion;

typedef struct {
  configuracion* configuracion;
  t_log* logger;
  char* pathPseudoCodigo;
} recursos;

recursos* generarLogger(char*);
void obtenerConfiguracion(recursos*, char*);
void obtenerPseudoCodigo(recursos*, char*);
void configuracion_consola(t_config *config);
void liberarRecursos(recursos* recursosConsola);

#endif
