#include <utils.h>
#include <recursos.h>
#include <conexiones.h>
#include <consola_conexiones.h>
#include <instrucciones/lista_instrucciones.h>

int main(int argc, char *argv[]) {
  //checkearArgumentosMain(argc);

  int socketKernel;
  t_paquete* paquete;
  t_list *instrucciones = list_create();

  inicializarRecursos("consola.log", argv[1], argv[2]);

  socketKernel = generarConexionConKernel();
  generarListaDeInstrucciones(instrucciones);
  paquete = enpaquetarInstrucciones(instrucciones);
  enviar_paquete(paquete, socketKernel);

  log_info(recursosConsola->logger, "Enviando lista de instrucciones al servidor Kernel.");
  //funcion para comprobar el handshake con Kernel
  //Logger para salir del Kernel
  //Terminar la consola
  //Liberar las instrucciones?

  //list_destroy(instrucciones);
  //liberarInstrucciones(instrucciones);
  liberarPaquete(paquete);
  liberarRecursos(recursosConsola);
  cerrarConexion(socketKernel);
}

