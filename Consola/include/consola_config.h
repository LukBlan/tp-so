#ifndef CONSOLA_CONFIG_H
#define CONSOLA_CONFIG_H
#include <commons/log.h>
#include <commons/config.h>

typedef struct
{
  char *IP_KERNEL;
  char *PUERTO_KERNEL;
} consolaConfig;

extern consolaConfig CONSOLA_CONFIG;
/**
 * @brief Rellena la variable CONSOLE_CONFIG con las configuraciones de la consola por archivo.
 *
 @param config Configuración.
 */
void configuracion_consola(t_config *config);

#endif
