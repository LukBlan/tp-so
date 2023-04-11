/*
 * main.c
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <consola_utils.h>
#include <commons/log.h>

int main(int argc, char *argv[])
{
  Config *config = config_create("Consola.config");
  Logger *logger = log_create("Consola.log", "Consola", true, LOG_LEVEL_INFO);

  log_info(logger, "Conectando con el Servidor Kernel...");
   int socket_kernel = conectar_kernel();

   if (socket_kernel < 0)
   {
     log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
     return EXIT_FAILURE;
   }

   log_info(logger, "Conexión exitosa. Iniciando cliente...");


}

