#include <commons/config.h>
#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>

t_recursos* recursosMemoria;

void inicializarRecursos(char* pathLogger, char* pathConfiguracion) {
  crearRecursosMemoria();
  cargarLogger(pathLogger);
  cargarConfiguracion(pathConfiguracion);
}

void crearRecursosMemoria() {
  recursosMemoria = malloc(sizeof(t_recursos));
  recursosMemoria->configuracion = NULL;
  recursosMemoria->logger = NULL;
  recursosMemoria->socketServidor = -1;
}

void cargarLogger(char* pathLogger) {
  recursosMemoria->logger = log_create(pathLogger, "Memoria", true, LOG_LEVEL_INFO);
}

void cargarConfiguracion(char* pathArchivoConfiguracion) {
  t_configuracion* configuracion = malloc(sizeof(t_configuracion));
  t_config* archivoConfiguracion = config_create(pathArchivoConfiguracion);
  configuracion->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(archivoConfiguracion, "PUERTO_ESCUCHA"));
  configuracion->IP_ESCUCHA = string_duplicate(config_get_string_value(archivoConfiguracion, "IP_ESCUCHA"));
  configuracion->TAM_MEMORIA = config_get_int_value(archivoConfiguracion, "TAM_MEMORIA");
  configuracion->TAM_SEGMENTO_0 = config_get_int_value(archivoConfiguracion, "TAM_SEGMENTO_0");
  configuracion->CANT_SEGMENTOS = config_get_int_value(archivoConfiguracion, "CANT_SEGMENTOS");
  configuracion->RETARDO_MEMORIA= config_get_int_value(archivoConfiguracion, "RETARDO_MEMORIA");
  configuracion->RETARDO_COMPACTACION = config_get_int_value(archivoConfiguracion, "RETARDO_COMPACTACION");
  configuracion->ALGOTIRMO_ASIGNACION = string_duplicate(config_get_string_value(archivoConfiguracion, "ALGORITMO_ASIGNACION"));
  config_destroy(archivoConfiguracion);
  recursosMemoria->configuracion = configuracion;
}
