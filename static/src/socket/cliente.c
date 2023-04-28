/*
 * cliente.c
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */
#include <socket/cliente.h>
uint32_t handshake = 1;
uint32_t result;
int crear_conexion_servidor(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
			 server_info->ai_socktype,
			 server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente,server_info->ai_addr,server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}
void handshake_cliente(int socket_cliente){
	send(socket_cliente, &handshake, sizeof(uint32_t), NULL);
	recv(socket_cliente, &result, sizeof(uint32_t), MSG_WAITALL);
	if(result == 0)
		   printf("Handshake realizado con exito");
		else
			printf("Handshake ha fallado");
}

void liberar_conexion_servidor(int socket_cliente)
{
	close(socket_cliente);
}

void terminar_programa(int conexion, t_config* config, t_log* logger)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion_servidor(conexion);

}
t_config* abrir_config(char *path)
{
	t_config* config;

	if ((config = config_create(path)) == NULL){
			printf("No se pudo abrir el Archivo");
			exit (2);
		}
	return config;
}


