#include <kernel_config.h>
#include <commons/config.h>
#include <commons/string.h>

configuracion* obtenerConfiguracion() {
  configuracion* config = malloc(sizeof(configuracion));

  t_config* fileConfig = config_create("Kernel.config");

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
  return config;
}
