#include <recursos.h>

t_recursos* recursosConsola;

t_recursos* generarLogger(char* pathLogger) {
  t_recursos* recursosConsola = malloc(sizeof(t_recursos));
  t_log *logger = log_create(pathLogger, "Consola", true, LOG_LEVEL_INFO);
  recursosConsola->logger = logger;
  return recursosConsola;
}

void obtenerConfiguracion(t_recursos* recursosConsola, char* pathConfiguracion) {
  t_config *archivoConfig = config_create(pathConfiguracion);
  t_configuracion* config = malloc(sizeof(t_configuracion));
  config->IP_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "IP_KERNEL"));
  config->PUERTO_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "PUERTO_KERNEL"));
  recursosConsola->configuracion = config;
  config_destroy(archivoConfig);
}

void obtenerPseudoCodigo(t_recursos* recursosConsola, char* pathPseudoCodigo) {
  recursosConsola->pathPseudoCodigo = pathPseudoCodigo;
}

void liberarRecursos(t_recursos* recursosConsola) {
  free(recursosConsola->configuracion->IP_KERNEL);
  free(recursosConsola->configuracion->PUERTO_KERNEL);
  free(recursosConsola->configuracion);
  log_destroy(recursosConsola->logger);
  free(recursosConsola);
}
