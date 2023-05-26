#include <socket/servidor.h>

int iniciar_servidor(char *IP,char *PUERTO) {
	int socket_servidor;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, PUERTO, &hints, &servinfo);

	socket_servidor = socket(servinfo->ai_family,
							 servinfo->ai_socktype,
							 servinfo->ai_protocol);

	int reuse_addr = 1;
	setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int));

	bind(socket_servidor,servinfo->ai_addr,servinfo->ai_addrlen);
	listen(socket_servidor,SOMAXCONN);
	freeaddrinfo(servinfo);
	return socket_servidor;
}

void handshake_servidor(int socket_cliente){
	uint32_t handshake;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;
	recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
	if (handshake == 1) {
		send(socket_cliente, &resultOk, sizeof(uint32_t), NULL);
	} else {
		send(socket_cliente, &resultError, sizeof(uint32_t), NULL);
	}
}

int esperar_cliente(int socket_servidor) {
	return accept(socket_servidor,NULL,NULL);
}

void apagar_servidor(int socketServidor) {
  close(socketServidor);
}

