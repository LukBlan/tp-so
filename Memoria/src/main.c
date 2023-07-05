#include <recursos.h>
#include <utils.h>
#include <memoria_conexiones.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursosMemoria();
  cargarLogger("memoria.log");
  cargarConfiguracion(argv[1]);
  iniciarEstructurasMemoria();
  cargarConexiones();

  montarServidor();

  liberarRecursos();
  return 0;
}
