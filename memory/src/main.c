#include <recursos.h>
#include <utils.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);
  inicializarRecursos("memory.log", argv[1]);
  puts("salio");
  //liberarRecursos();
  return 0;
}
