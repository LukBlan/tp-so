#include <recursos.h>
#include <stdlib.h>

t_recursos* recursosConsola;

void inicializarRecursos(char* pathLoger, char* pathConfiguracion, char* pathPseudoCodigo) {
  recursosConsola = malloc(sizeof(t_recursos));
  cargarLogger(pathLoger);
  cargarConfiguracion(pathConfiguracion);
  cargarPseudoCodigo(pathPseudoCodigo);
}

void cargarLogger(char* pathLogger) {
  recursosConsola->logger = log_create(pathLogger, "Consola", true, LOG_LEVEL_INFO);
}

void cargarConfiguracion(char* pathConfiguracion) {
  t_config *archivoConfig = config_create(pathConfiguracion);
  t_configuracion* config = malloc(sizeof(t_configuracion));
  config->IP_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "IP_KERNEL"));
  config->PUERTO_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "PUERTO_KERNEL"));
  recursosConsola->configuracion = config;
  config_destroy(archivoConfig);
}

void cargarPseudoCodigo(char* pathPseudoCodigo) {
  recursosConsola->pathPseudoCodigo = pathPseudoCodigo;
}

void liberarRecursos() {
  free(recursosConsola->configuracion->IP_KERNEL);
  free(recursosConsola->configuracion->PUERTO_KERNEL);
  free(recursosConsola->configuracion);
  log_destroy(recursosConsola->logger);
  free(recursosConsola);
}
