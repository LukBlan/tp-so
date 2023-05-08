#include <commons/string.h>
#include <cpu_conexion.h>
#include <socket/servidor.h>

int generar_servidor(char* ip, int puerto) {
  char* puertoServidor = string_itoa(puerto);
  int socketServidor = iniciar_servidor(ip, puertoServidor);
  return socketServidor;
}
