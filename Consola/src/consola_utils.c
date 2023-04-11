#include <consola_utils.h>

int conectar_kernel()
{
  return crear_conexion_con_servidor(CONSOLA_CONFIG.IP_KERNEL, CONSOLA_CONFIG.PUERTO_KERNEL);
}
