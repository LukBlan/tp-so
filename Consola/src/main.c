#include <consola_utils.h>

int main(int argc, char *argv[]) {
  recursos* recursosConsola;
  int socketKernel;
  t_list *instrucciones;

  recursosConsola = generarLogger("consola.log");
  checkearArgumentosMain(recursosConsola->logger, argc);
  obtenerConfiguracion(recursosConsola, argv[1]);
  obtenerPseudoCodigo(recursosConsola, argv[2]);
  socketKernel = generarConexionConKernel(recursosConsola);
  instrucciones = generarListaDeInstrucciones(recursosConsola);

  Paquete* paquete = crear_paquete(LINEAS_INSTRUCCION);
  //serializar_lista_instruc(paquete,instrucciones);
  //enviar_paquete(paquete,socket_kernel);
  log_info(recursosConsola->logger,"Enviando lista de instrucciones al servidor Kernel.");

  //funcion para comprobar el handshake con Kernel
  //Logger para salir del Kernel
  //Terminar la consola
}

