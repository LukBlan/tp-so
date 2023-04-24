#include <sys/socket.h>
#include <commons/log.h>
#include "../include/estructuras.h"

infoConexion obtenerInfoDeConexion(char*);
int crearSocket(t_log*);
void conectarConModulo(int socket, char*);
struct addrinfo* obtenerServidor(infoConexion infoConexion);
