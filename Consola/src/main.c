/*
 * main.c
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */
#include <consola_utils.h>
#include <commons/log.h>

int main(int argc, char *argv[])
{
  char *direccionConfig = argv[1];
  t_config *config = fopen(direccionConfig,"r");
  t_logger *logger = log_create("Consola.log", "Consola", true, LOG_LEVEL_INFO);
  if(argc!=1 && argc!=2){
	  log_error(logger,"No se ingreso la cantidad de argumentos necesaria");
	  exit(EXIT_FAILURE);
  }

  log_info(logger, "Conectando con el Servidor Kernel...");
   int socket_kernel = conectar_kernel();

   if (socket_kernel < 0)
   {
     log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
     return EXIT_FAILURE;
   }

   log_info(logger, "Conexión exitosa. Iniciando cliente...");
   handshake_cliente(socket_kernel);

}

