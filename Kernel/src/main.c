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

t_queue* crearListaDeRecursos(void){
	t_queue* listaRecursos = queue_create();// creo la lista de recursos
	return listaRecursos;
}

t_queue* crearListaDeInstancias(void){
	t_queue* listaInstanciaRecursos = queue_create();// creo la lista de recursos
	return listaInstanciaRecursos;
}


t_queue* cargarListaDeRecursos(void){
	t_queue* listaRecursos = crearListaDeRecursos();

	if(queue_is_empty(listaRecursos)){ // valido que este vacia si lo esta voy a cargar todos los recursos que vienen en el config
		t_configuracion* instancias;
		int cantRecursos = list_size(instancias->RECURSOS);

		for (size_t i = 0; i < cantRecursos; i++) {

			int auxRecursos = list_get(instancias->RECURSOS, i);
			queue_push(listaRecursos, auxRecursos);
			i+=1;
		}
	}
	return listaRecursos;
}

t_queue* cargarListaDeInstancias(void){

	t_queue* listaInstancias = crearListaDeInstancias();
	if(queue_is_empty(listaInstancias)){ // valido que este vacia si lo esta voy a cargar todos los recursos que vienen en el config
		t_configuracion* instancias;
		int cantInstancias = list_size(instancias->INSTANCIAS_RECURSOS);

		for (size_t i = 0; i < cantInstancias; i++) {

			int auxInstancia = list_get(instancias->INSTANCIAS_RECURSOS, i);
			queue_push(listaInstancias, auxInstancia);
			i+=1;
		}
	}
	return listaInstancias;
}

void validoExistenciaDeRecurso(t_queue* listaRecursos/*,recursopedido*/){
	t_queue* auxListaRecursos = queue_create();
	t_queue* listainstancias = cargarListaDeInstancias();
	char* recurso = "computadora";

	while (!queue_is_empty(listaRecursos)) {
		void *elemento = queue_peek(listaRecursos);

	    // Copiar el elemento a la cola copiada
	    queue_push(auxListaRecursos, elemento);

	    // Eliminar el elemento de la cola original
	    queue_pop(auxListaRecursos);
	}

	if(!queue_is_empty(listaRecursos)){
		queue_clean(listaRecursos);
	}

	int cantRecursos = queue_size(auxListaRecursos);// obtengo la cantidad de recursos
	int existeRecurso = 0;
	for (size_t i = 0; i < cantRecursos; i++) { // recorro la lista y valida que exsista el recurso pedido en la lista de recursos
		// Obtener el elemento en el frente de la cola
		//void * elemento = queue_peek(listaRecursos);
		char* elemento =  queue_peek(auxListaRecursos);
		if (recurso == elemento){
			existeRecurso = 1;
			procesarRecursoWait(existeRecurso,listainstancias);
		}
		i+=1;
	}
	if (existeRecurso != 1){
		//sino existe el recurso lo mando al proceso en EXIT
	}

}

void procesarRecursoWait(int existeRecurso, t_queue* listainstancias){
	if (existeRecurso == 1){

		int cantidadInstancias = queue_size(listainstancias);
		if(cantidadInstancias > 0){
			  //resto 1 a la cantidad de recursos disponibles
		}
		else {
			  //bloqueo el recurso solicitado
		}
	}
}

void procesarRecursoSignal(int existeRecurso, t_queue* listainstancias){
	if (existeRecurso == 1){
		//sumo 1 a la cantidad de recursos disponibles
		// desbloqueo el primer elemento de la cola de bloqeados d ese recurso.
		// se devuelve la ejecución al proceso que peticionó el SIGNAL.
	}
}

