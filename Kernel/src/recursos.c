#include <commons/config.h>
#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>

t_recursos* recursosKernel;

void crearRecursos() {
  recursosKernel = malloc(sizeof(t_recursos));
  recursosKernel->configuracion = NULL;
  recursosKernel->conexiones = malloc(sizeof(t_conexiones));
  recursosKernel->conexiones->socketCpu = -1;
  recursosKernel->conexiones->socketFileSystem = -1;
  recursosKernel->conexiones->socketMemoria = -1;
}

void cargarConfiguracion(char* pathConfiguracion) {
  t_configuracion* config = malloc(sizeof(t_configuracion));
  t_config* fileConfig = config_create(pathConfiguracion);

  config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
  config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
  config->PUERTO_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_MEMORIA"));
  config->IP_FILESYSTEM = string_duplicate(config_get_string_value(fileConfig, "IP_FILESYSTEM"));
  config->PUERTO_FILESYSTEM = string_duplicate(config_get_string_value(fileConfig, "PUERTO_FILESYSTEM"));
  config->IP_CPU = string_duplicate(config_get_string_value(fileConfig, "IP_CPU"));
  config->PUERTO_CPU = string_duplicate(config_get_string_value(fileConfig, "PUERTO_CPU"));
  config->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_ESCUCHA"));
  config->ALGORITMO_PLANIFICACION = config_get_string_value(fileConfig,"ALGORITMO_PLANIFICACION");
  config->ESTIMACION_INICIAL = config_get_int_value(fileConfig,"ESTIMACION_INICIAL");
  config->HRRN_ALFA = config_get_int_value(fileConfig,"HRRN_ALFA");
  config->GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(fileConfig,"GRADO_MAX_MULTIPROGRAMACION");
  config->RECURSOS = config_get_int_value(fileConfig,"RECURSOS");//es una lista de char
  config->INSTANCIAS_RECURSOS = config_get_int_value(fileConfig,"INSTANCIAS_RECURSOS");// es una lista de int

  config_destroy(fileConfig);
  recursosKernel->configuracion = config;
}

void cargarLogger(char* pathLogger) {
  recursosKernel->logger = log_create(pathLogger, "Kernel", true, LOG_LEVEL_INFO);
}
