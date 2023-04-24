#include <stdlib.h>
#include "../include/conexion.h"
#include <netdb.h>
#include <commons/config.h>
#include <commons/string.h>


int crearSocket(t_log* logger) {
	int nuevoSocket;
	if ((nuevoSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	  log_error(logger, "No se pudo crear el socket correctamente");
	  exit(EXIT_FAILURE);
	}
	return nuevoSocket;
}

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

void conectarConModulo(int socket, char* nombreModulo) {
  int estado;
  infoConexion infoConexion = obtenerInfoDeConexion(nombreModulo);
  struct addrinfo* modulo = obtenerServidor(infoConexion);
  if ((estado = connect(socket, modulo->ai_addr, modulo->ai_addrlen)) < 0) {
    printf("\nFallo de Conexion\n");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(modulo);
}

struct addrinfo* obtenerServidor(infoConexion infoConexion) {
  struct addrinfo hints;
  struct addrinfo *serverInfo;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  getaddrinfo(infoConexion.ip, infoConexion.puerto, &hints, &serverInfo);
  return serverInfo;
}
