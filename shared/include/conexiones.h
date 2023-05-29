#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int crear_conexion_servidor(char* ip, char* puerto);
void liberar_conexion_servidor(int socket_cliente);
int iniciar_servidor(char* ip,char* puerto);
int esperar_cliente(int socket_servidor);
void cerrarConexion(int socket);

