#include <kernel_conexiones.h>
#include <stdio.h>
#include <recursos.h>
#include <pthread.h>
#include <utils.h>
#include <estructuras.h>

int main(int argc, char* argv[]) {

	//t_recurso* recurso;

  //espero a que me pidan un recurso

  //int auxExisteRecurso = validoExistenciaDeRecurso(listaDeRecursos/*,recursopedido*/);

 // queue_clean(listaDeRecursos);

  /*if ( cantRecursos > 0){
	  queue_push(listaRecursos, 4);
  }
  */
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
