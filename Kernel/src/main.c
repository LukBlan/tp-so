#include <kernel_conexiones.h>
#include <stdio.h>
#include <recursos.h>
#include <pthread.h>
#include <utils.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

  crearRecursos();
  cargarConfiguracion(argv[1]);
  cargarLogger("kernel.log");
  cargarConexiones();
  iniciarColas();
  iniciarSemaforos();
  comenzarPlanificadores();

  montarServidor();

  free(recursosKernel);
  return EXIT_SUCCESS;
}

