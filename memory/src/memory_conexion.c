#include <commons/string.h>
#include <memory_conexion.h>


void montar_servidor(configuracion* config) {
	int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);
		while (1) {
			int socketCliente = esperarCliente(socketServidor);
				log_info(logger, "Recibi un cliente");

					}
				close(socketServidor);
	}
