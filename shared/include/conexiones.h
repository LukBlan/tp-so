#ifndef CONEXIONES_H
#define CONEXIONES_H

  #include <string.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <commons/log.h>
  #include <estructuras.h>

  int crearConexionServidor(char *ip, char* puerto);
	int iniciarServidor(char*ip, char* puerto);
	int esperarCliente(int socketServidor);
	void cerrarConexion(int socketServidor);
	int realizarHandshake(t_log* logger, int socketCliente);
	t_buffer* obtenerBuffer(int socketCliente);
	op_code obtenerCodigoOperacion(int socketCliente);
	void enviar_paquete(t_paquete* paquete, int socketCliente);

#endif

