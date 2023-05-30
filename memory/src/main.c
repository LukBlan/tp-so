#include <recursos.h>
#include <utils.h>
#include <memory_conexiones.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursos();
  cargarLogger("memory.log");
  cargarConfiguracion(argv[1]);
  cargarConexiones();

  montarServidor();

  liberarRecursos();
  return 0;
}
