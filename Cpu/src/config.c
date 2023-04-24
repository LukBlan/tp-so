#include "../include/config.h"
#include <commons/config.h>
#include <commons/string.h>

infoConexion obtenerInfoDeConexion(char* modulo) {
  t_config* archivoConfiguracion = config_create("cpu.config");
  infoConexion infoConexion;
  char* ipModulo = string_from_format("IP_%s", modulo);
  char* puertoModulo = string_from_format("PUERTO_%s", modulo);
  infoConexion.ip = string_duplicate(config_get_string_value(archivoConfiguracion, ipModulo));
  infoConexion.puerto = string_duplicate(config_get_string_value(archivoConfiguracion, puertoModulo));
  config_destroy(archivoConfiguracion);
  return infoConexion;
}

