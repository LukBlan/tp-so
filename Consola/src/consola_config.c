#include <consola_config.h>

void configuracion_consola(t_config *config)
{
  CONSOLA_CONFIG.IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
  CONSOLA_CONFIG.PUERTO_KERNEL = config_get_string_value(config, "PUERTO_KERNEL");
}
