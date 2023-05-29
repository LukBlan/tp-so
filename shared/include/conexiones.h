#ifndef CONEXIONES_H
#define CONEXIONES_H

  #include <string.h>
  #include <stdlib.h>
  #include <unistd.h>

  int crearConexionServidor(char* ip, char* puerto);
  int iniciarServidor(char* ip, char* puerto);
  int esperarCliente(int socketServidor);
  void cerrarConexion(int socket);

#endif

