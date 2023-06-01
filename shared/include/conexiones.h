#ifndef CONEXIONES_H
#define CONEXIONES_H

  #include <string.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <commons/log.h>

  int crearConexionServidor(char* ip, char* puerto);
  int iniciarServidor(char* ip, char* puerto);
  int esperarCliente(int socketServidor);
  void cerrarConexion(int socket);
  int realizarHandshake(t_log* logger, int socketCliente);

#endif

