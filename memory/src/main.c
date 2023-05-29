#include <recursos.h>
#include <utils.h>
#include <memory_conexiones.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);
  inicializarRecursos("memory.log", argv[1]);
  montarServidor();
  //liberarRecursos();
  return 0;
}
