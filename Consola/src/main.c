#include <consola_utils.h>

int main(int argc, char *argv[]) {
  recursos* recursosConsola = generarLogger("consola.log");
  checkearArgumentosMain(recursosConsola->logger, argc);
  obtenerConfiguracion(recursosConsola, argv[1]);
  obtenerPseudoCodigo(recursosConsola, argv[2]);

  log_info(recursosConsola->logger, "Conectando con el Servidor Kernel...");
  int socket_kernel = conectar_kernel(recursosConsola);
   if (socket_kernel < 0) {
     log_error(recursosConsola->logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
     return EXIT_FAILURE;
   }

   log_info(recursosConsola->logger, "Conexión exitosa. Iniciando cliente...");
   FILE *archivoDeInstrucciones = fopen (recursosConsola->pathPseudoCodigo, "r");

   if(archivoDeInstrucciones == NULL){
	   log_error (recursosConsola->logger,"El archivo no se pudo abrir");
	   exit(EXIT_FAILURE);
   }

   t_list *instrucciones = list_create(); // reserva espacio en memoria para la lista
   generar_lista_instrucciones(instrucciones , archivoDeInstrucciones);
   Paquete* paquete = crear_paquete(LINEAS_INSTRUCCION);
   //serializar_lista_instruc(paquete,instrucciones);
   //enviar_paquete(paquete,socket_kernel);
   log_info(recursosConsola->logger,"Enviando lista de instrucciones al servidor Kernel.");

   //funcion para comprobar el handshake con Kernel
   //Logger para salir del Kernel
   //Terminar la consola
}

