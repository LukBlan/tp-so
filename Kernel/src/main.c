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
 // t_configuracion* t_config = recursosKernel->configuracion;
  int cantRecursos = 3;//list_size(t_config->INSTANCIAS_RECURSOS);
  printf("la cantidad de recursos es %d\n",cantRecursos);
  montarServidor();

  liberarRecursos();
  return 0;
}

