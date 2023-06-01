#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>

int crearConexionServidor(char *ip, char* puerto) {
  struct addrinfo hints;
  struct addrinfo *serverInfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(ip, puerto, &hints, &serverInfo);

  int socketCliente = socket(serverInfo->ai_family,
      serverInfo->ai_socktype,
      serverInfo->ai_protocol);

  int estadoConexion = connect(socketCliente, serverInfo->ai_addr, serverInfo->ai_addrlen);
  freeaddrinfo(serverInfo);
  return (estadoConexion < 0)? estadoConexion : socketCliente;
}

int iniciarServidor(char*ip, char* puerto) {
  int socketServidor;
  struct addrinfo hints;
  struct addrinfo *serverInfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, puerto, &hints, &serverInfo);

  socketServidor = socket(serverInfo->ai_family,
      serverInfo->ai_socktype,
      serverInfo->ai_protocol);

  int reuse_addr = 1;
  setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int));

  bind(socketServidor, serverInfo->ai_addr, serverInfo->ai_addrlen);
  listen(socketServidor,SOMAXCONN);
  freeaddrinfo(serverInfo);
  return socketServidor;
}

int esperarCliente(int socketServidor) {
  return accept(socketServidor, NULL, NULL);
}

void cerrarConexion(int socketServidor) {
  close(socketServidor);
}

int realizarHandshake(t_log* logger, int socketCliente) {
  int informacionEnviada = 4;
  int informacionRecivida = 0;
  op_code codigoOperacion = HANDSHAKE;
  int resultadoHanshake = 0;

  send(socketCliente, &codigoOperacion, sizeof(op_code), 0);
  send(socketCliente, &informacionEnviada, sizeof(op_code), 0);
  recv(socketCliente, &informacionRecivida, sizeof(int), 0);

  if (informacionEnviada == informacionRecivida) {
    log_info(logger, "El Handshake fue exitoso");
    resultadoHanshake = 1;
  } else {
    log_error(logger, "Error el Handshake no fue exitoso");
    resultadoHanshake = -1;
  }
  return resultadoHanshake;
}
