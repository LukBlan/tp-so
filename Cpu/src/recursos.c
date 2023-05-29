#include <commons/config.h>
#include <commons/string.h>
#include <cpu_recursos.h>
#include <socket/cliente.h>

configuracion* obtenerConfiguracion(char* pathArchivo) {
  configuracion* config = malloc(sizeof(configuracion));
  t_config* fileConfig = abrir_config(pathArchivo);
  config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
  config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
  config->TAM_MAX_SEGMENTO = config_get_int_value(fileConfig, "TAM_MAX_SEGMENTO");
  config->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_ESCUCHA"));
  config->PUERTO_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_MEMORIA"));
  config->RETARDO_INSTRUCCION = config_get_int_value(fileConfig, "RETARDO_INSTRUCCION");
  config_destroy(fileConfig);
  return config;
}

recursos* obtenerRecursosModulo(char* pathConfiguracion, char* pathLogger) {
	recursos* recursosModulo = malloc(sizeof(recursos));
	t_log* logger = log_create(pathLogger, "CPU", 1, LOG_LEVEL_INFO);
	configuracion* configuracion = obtenerConfiguracion(pathConfiguracion);
	recursosModulo->configuracion = configuracion;
	recursosModulo->logger = logger;
	return recursosModulo;
}

void liberarRecursos(recursos* recursosModulo) {
  free(recursosModulo->configuracion->PUERTO_ESCUCHA);
  free(recursosModulo->configuracion->PUERTO_MEMORIA);
  free(recursosModulo->configuracion->IP_ESCUCHA);
  free(recursosModulo->configuracion->IP_MEMORIA);
  free(recursosModulo->configuracion);
  log_destroy(recursosModulo->logger);
  free(recursosModulo);
}
