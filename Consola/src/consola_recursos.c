#include <consola_recursos.h>
#include <commons/config.h>

recursos* generarLogger(char* pathLogger) {
  recursos* recursosConsola = malloc(sizeof(recursos));
  t_log *logger = log_create(pathLogger, "Consola", true, LOG_LEVEL_INFO);
  recursosConsola->logger = logger;
  return recursosConsola;
}

void obtenerConfiguracion(recursos* recursosConsola, char* pathConfiguracion) {
  t_config *archivoConfig = config_create(pathConfiguracion);
  configuracion* configuracion = malloc(sizeof(configuracion));
  configuracion->IP_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "IP_KERNEL"));
  configuracion->PUERTO_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "PUERTO_KERNEL"));
  recursosConsola->configuracion = configuracion;
  config_destroy(archivoConfig);
}

void obtenerPseudoCodigo(recursos* recursosConsola, char* pathPseudoCodigo) {
  recursosConsola->pathPseudoCodigo = pathPseudoCodigo;
}
