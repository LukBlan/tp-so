#ifndef CLIENTE_H
#define CLIENTE_H

#include <socket/socket.h>

/**
 * @resumen Crea una conexión con un servidor.
 * @parametros  IP del servidor.
 * @parametros  Puerto del servidor.
 * @return Socket del cliente.
 */
int crear_conexion_servidor(char *ip, char *puerto);

/**
 * @resumen Libera la conexión con el servidor.
 *
 * @parametros socketCliente Socket del cliente.
 */
void liberar_conexion_servidor(int socketCliente);

/**
 * @resumen Termina un programa liberando parametros.
 *
 * @param socket .
 * @param config.
 * @param logger.
 */
void terminar_programa(int socket, t_config *config, t_log *logger);

t_config* abrir_config(char *path);

#endif
