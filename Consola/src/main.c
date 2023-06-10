#include <recursos.h>
#include <consola_conexiones.h>
#include <utils.h>
#include <instrucciones/lista_instrucciones.h>
#include <estructuras.h>

int main(int argc, char* argv[]) {
  t_list* instrucciones = list_create();

  validarCantidadArgumentosMain(argc, 3);

  crearRecursosConsola();
  cargarLogger("consola.log");
  cargarConfiguracion(argv[1]);
  cargarPseudoCodigo(argv[2]);
  //cargarConexionConKernel();

  generarListaDeInstrucciones(instrucciones);
  mostrarInstrucciones(instrucciones);
  //enviarInstrucciones(instrucciones);
  liberarInstrucciones(instrucciones);
  liberarRecursos();
}

