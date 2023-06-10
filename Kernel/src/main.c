#include <kernel_conexiones.h>
#include <stdio.h>
#include <recursos.h>
#include <pthread.h>
#include <utils.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursos();
  cargarLogger("kernel.log");
  cargarConfiguracion(argv[1]);
  cargarConexiones();
  iniciarColas();
  iniciarSemaforos();
  comenzarPlanificadores();

  montarServidor();

  liberarRecursos();
  return 0;
}

