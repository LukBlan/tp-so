#include <stdlib.h>
#include "../include/conexion.h"
#include "../include/logger.h"
#include <netdb.h>
#include <commons/config.h>
#include <commons/string.h>
#include <string.h>

typedef struct addrinfo addrServer;

int crearSocket(t_log* logger, addrServer* servidor) {
	int nuevoSocket;
	if ((socket(servidor->ai_family, servidor->ai_socktype, servidor->ai_protocol)) < 0) {
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

int conectarConServidor(char* nombreModulo) {
  t_log* logger= crearLogger("Test1");
  int estado;
  infoConexion infoConexion = obtenerInfoDeConexion(nombreModulo);
  struct addrinfo* servidor = obtenerServidor(infoConexion);
  int socketCliente = crearSocket(logger, servidor);
  if ((estado = connect(socketCliente, servidor->ai_addr, servidor->ai_addrlen)) < 0) {
    printf("\nFallo de Conexion\n");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(servidor);
  //log_destroy(logger);
  return socketCliente;
}

struct addrinfo* obtenerServidor(infoConexion infoConexion) {
  struct addrinfo hints;
  struct addrinfo *serverInfo;
  char* ip = infoConexion.ip;
  char* puerto = infoConexion.puerto;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo("0.0.0.0", "35762", &hints, &serverInfo);
  return serverInfo;
}

int crearServidor(char* nombreModulo) {
  infoConexion infoConexion = obtenerInfoDeConexion(nombreModulo);
  struct addrinfo* servidor = obtenerServidor(infoConexion);
  int socketServidor = crearSocket(crearLogger("Test"), servidor);
  bind(socketServidor, servidor->ai_addr, servidor->ai_addrlen);
  listen(socketServidor, SOMAXCONN);
  freeaddrinfo(servidor);
  //log_destroy(logger);
  return socketServidor;
}
