/*
 * main.c
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */
#include <consola_utils.h>
#include <commons/log.h>

int main(int argc, char *argv[]) {
  char *direccionConfig = argv[1];
  t_config *config = abrir_config(direccionConfig);
  t_log *logger = log_create("Consola.log", "Consola", true, LOG_LEVEL_INFO);
  if(argc!=3){
	  log_error(logger,"No se ingreso la cantidad de argumentos necesaria");
	  exit(EXIT_FAILURE);
  }
  configuracion_consola(config);
  char *direccionPseudocodigo = argv[2];
  log_info(logger, "Conectando con el Servidor Kernel...");
  int socket_kernel = conectar_kernel();
  puts("Me intente conectar");
   if (socket_kernel < 0)
   {
     log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
     return EXIT_FAILURE;
   }

   log_info(logger, "Conexión exitosa. Iniciando cliente...");
   FILE *instrucciones = fopen (direccionPseudocodigo ,"r");
   if(instrucciones == NULL){
	   log_error (logger,"El archivo no se pudo abrir");
	   exit(EXIT_FAILURE);
   }
   t_list *listaInstrucciones = list_create(); // reserva espacio en memoria para la lista
   generar_lista_instrucciones(listaInstrucciones , instrucciones);
   Paquete* paquete = crear_paquete(LINEAS_INSTRUCCION);
   serializar_lista_instruc(paquete,listaInstrucciones);
   enviar_paquete(paquete,socket_kernel);
   log_info(logger,"Enviando lista de instrucciones al servidor Kernel.");
   //funcion para comprobar el handshake con Kernel
   //Logger para salir del Kernel
   //Terminar la consola
}

