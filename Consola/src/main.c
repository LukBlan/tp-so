#include <recursos.h>
#include <consola_conexiones.h>
#include <consola_utils.h>
#include <utils.h>
#include <instrucciones/lista_instrucciones.h>

int main(int argc, char* argv[]) {
  t_paquete* paquete;
    t_list* instrucciones;

  validarCantidadArgumentosMain(argc, 3);

  crearRecursosConsola();
  cargarLogger("consola.log");
  cargarConfiguracion(argv[1]);
  cargarPseudoCodigo(argv[2]);
  cargarConexionConKernel();

  instrucciones = generarListaDeInstrucciones();
  paquete = enpaquetarInstrucciones(instrucciones);
  enviar_paquete(paquete, recursosConsola->conexiones->socketKernel);

  log_info(recursosConsola->logger, "Enviando lista de instrucciones al servidor Kernel.");
  //funcion para comprobar el handshake con Kernel
  //Liberar las instrucciones?
  //list_destroy(instrucciones);
  //liberarInstrucciones(instrucciones);
  liberarPaquete(paquete);
  liberarRecursos();
}

