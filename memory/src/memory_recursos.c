#include <commons/config.h>
#include <commons/string.h>
#include <memory_recursos.h>
#include <stdlib.h>

void inicializarRecursos(char* pathLogger, char* pathConfiguracion) {

}

t_configuracion* obtenerConfiguracion(char* pathArchivo){
  t_configuracion* configuracion = malloc(sizeof(configuracion));
  t_config* archivoConfiguracion = config_create("memory.config");
  configuracion->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(archivoConfiguracion, "PUERTO_ESCUCHA"));
  configuracion->TAM_MEMORIA = config_get_int_value(archivoConfiguracion, "TAM_MEMORIA");
  configuracion->TAM_SEGMENTO_0 = config_get_int_value(archivoConfiguracion, "TAM_SEGMENTO_0");
  configuracion->CANT_SEGMENTOS = config_get_int_value(archivoConfiguracion, "CANT_SEGMENTOS");
  configuracion->RETARDO_MEMORIA= config_get_int_value(archivoConfiguracion, "RETARDO_MEMORIA");
  configuracion->RETARDO_COMPACTACION = config_get_int_value(archivoConfiguracion, "RETARDO_COMPACTACION");
  configuracion->ALGOTIRMO_ASIGNACION = string_duplicate(config_get_string_value(archivoConfiguracion, "ALGORITMO_ASIGNACION"));
  config_destroy(archivoConfiguracion);
  return configuracion;
}
