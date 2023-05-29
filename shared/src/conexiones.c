#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int crear_conexion_servidor(char *ip, char* puerto) {
  struct addrinfo hints;
  struct addrinfo *serverInfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(ip, puerto, &hints, &serverInfo);

  int socket_cliente = socket(serverInfo->ai_family,
      serverInfo->ai_socktype,
      serverInfo->ai_protocol);

  connect(socket_cliente, serverInfo->ai_addr, serverInfo->ai_addrlen);
  freeaddrinfo(serverInfo);
  return socket_cliente;
}

int iniciar_servidor(char*ip, char* puerto) {
  int socket_servidor;
  struct addrinfo hints;
  struct addrinfo *serverInfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, puerto, &hints, &serverInfo);

  socket_servidor = socket(serverInfo->ai_family,
      serverInfo->ai_socktype,
      serverInfo->ai_protocol);

  int reuse_addr = 1;
  setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int));

  bind(socket_servidor, serverInfo->ai_addr, serverInfo->ai_addrlen);
  listen(socket_servidor,SOMAXCONN);
  freeaddrinfo(serverInfo);
  return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
  return accept(socket_servidor, NULL, NULL);
}

void cerrarConexion(int socketServidor) {
  close(socketServidor);
}

