#include <consola_utils.h>

int main(int argc, char *argv[]) {
  int socketKernel;
  paquete* paquete;
  t_list *instrucciones;
  recursos* recursosConsola;

  recursosConsola = generarLogger("consola.log");
  checkearArgumentosMain(recursosConsola->logger, argc);
  obtenerConfiguracion(recursosConsola, argv[1]);
  obtenerPseudoCodigo(recursosConsola, argv[2]);

  socketKernel = generarConexionConKernel(recursosConsola);
  instrucciones = generarListaDeInstrucciones(recursosConsola);
  paquete = enpaquetarInstrucciones(instrucciones);

  //enviar_paquete(paquete, socketKernel);
  log_info(recursosConsola->logger,"Enviando lista de instrucciones al servidor Kernel.");
  //funcion para comprobar el handshake con Kernel
  //Logger para salir del Kernel
  //Terminar la consola
  liberarRecursos(recursosConsola);
}

