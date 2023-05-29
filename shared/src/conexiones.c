#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int crearConexionServidor(char *ip, char* puerto) {
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

