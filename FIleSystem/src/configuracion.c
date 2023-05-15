#include "../include/configuracion.h"
#include <commons/config.h>
#include <commons/string.h>

configuracion obtenerConfiguracion() {
  configuracion configuracion;
  t_config* archivoConfiguracion = config_create("cpu.config");
  configuracion.IP_MEMORIA = string_duplicate(config_get_string_value(archivoConfiguracion, "IP_MEMORIA"));
  configuracion.TAM_MAX_SEGMENTO = config_get_int_value(archivoConfiguracion, "TAM_MAX_SEGMENTO");
  configuracion.PUERTO_ESCUCHA = config_get_int_value(archivoConfiguracion, "PUERTO_ESCUCHA");
  configuracion.PUERTO_MEMORIA = config_get_int_value(archivoConfiguracion, "PUERTO_MEMORIA");
  configuracion.RETARDO_INSTRUCCION = config_get_int_value(archivoConfiguracion, "RETARDO_INSTRUCCION");
  config_destroy(archivoConfiguracion);
  return configuracion;
}
