#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>
#include <string.h>
#include <serializacion/paquete.h>
#include <serializacion/contexto.h>
#include <serializacion/buffer.h>

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

t_buffer* obtenerBuffer(int socketCliente) {
  int tamanioBuffer;
  recv(socketCliente, &tamanioBuffer, sizeof(int), 0);
  void* stream = malloc(tamanioBuffer);
  recv(socketCliente, stream, tamanioBuffer, 0);
  t_buffer* buffer = malloc(tamanioBuffer + sizeof(int));
  buffer->stream = stream;
  buffer->size = tamanioBuffer;
  return buffer;
}

op_code obtenerCodigoOperacion(int socketCliente) {
  op_code codigoOperacion;
  if (recv(socketCliente, &codigoOperacion, sizeof(int), MSG_WAITALL) > 0) {
    printf("codigo de Operacion: %d\n", codigoOperacion);
    return codigoOperacion;
  } else {
    puts("DESCONEXION");
    close(socketCliente);
    return DESCONEXION;
  }
}

void enviar_paquete(t_paquete* paquete, int socketCliente) {
  int bytes = paquete->buffer->size + 2 * sizeof(int);
  void* a_enviar = serializar_paquete(paquete, bytes);
  send(socketCliente, a_enviar, bytes, 0);
  free(a_enviar);
}

void enviarContexto(contextoEjecucion* contexto, int socketCpu, op_code codigoOperacion) {
  int tamanioContexto = tamanioBytesContexto(contexto);
  t_buffer* buffer = generarBuffer(tamanioContexto);

  serializarContexto(buffer, contexto);
  t_paquete* paquete = crearPaquete(buffer, codigoOperacion);
  enviar_paquete(paquete, socketCpu);
}

contextoEjecucion* recibirContexto(int socketCliente) {
  t_buffer* buffer = obtenerBuffer(socketCliente);
  contextoEjecucion* contexto = deserializarContexto(buffer);
  return contexto;
}
