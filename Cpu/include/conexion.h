#include <sys/socket.h>
#include <commons/log.h>
#include "../include/estructuras.h"
#include <netdb.h>

typedef struct addrinfo addrServer;

infoConexion obtenerInfoDeConexion(char*);
int crearSocket(t_log*, addrServer*);
int conectarConServidor(char*);
struct addrinfo* obtenerServidor(infoConexion infoConexion);
int crearServidor(char*);
