#include <pthread.h>
#include <cpu_conexion.h>
#include <utils.h>
#include <recursos.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursosCpu();
  cargarLogger("cpu.log");
  cargarConfiguracion(argv[1]);
  cargarConexiones();

  montarServidor();

  liberarRecursos();
  return 0;
}
