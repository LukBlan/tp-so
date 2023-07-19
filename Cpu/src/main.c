#include <pthread.h>
#include <cpu_conexion.h>
#include <utils.h>
#include <recursos.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);
  agarrarSenial();
  crearRecursosCpu();
  cargarLogger("cpu.log");
  cargarConfiguracion(argv[1]);
  cargarConexiones();

  realizarHandshakeMemoria();
  montarServidor();

  liberarRecursos();
  return 0;
}
