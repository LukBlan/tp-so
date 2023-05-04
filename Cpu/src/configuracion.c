#include "../include/configuracion.h"
#include <commons/config.h>
#include <commons/string.h>

configuracion obtenerConfiguracion() {
  configuracion configuracion;
  t_config* archivoConfiguracion = config_create("cpu.config");
  config_destroy(archivoConfiguracion);
  return configuracion;
}
