#include <cpu_config.h>
#include <commons/config.h>
#include <commons/string.h>

configuracion* obtenerConfiguracion() {
  configuracion* config = malloc(sizeof(configuracion));
  t_config* fileConfig = config_create("cpu.config");
  config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
  config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
  config->TAM_MAX_SEGMENTO = config_get_int_value(fileConfig, "TAM_MAX_SEGMENTO");
  config->PUERTO_ESCUCHA = config_get_int_value(fileConfig, "PUERTO_ESCUCHA");
  config->PUERTO_MEMORIA = config_get_int_value(fileConfig, "PUERTO_MEMORIA");
  config->RETARDO_INSTRUCCION = config_get_int_value(fileConfig, "RETARDO_INSTRUCCION");
  config_destroy(fileConfig);
  return config;
}
