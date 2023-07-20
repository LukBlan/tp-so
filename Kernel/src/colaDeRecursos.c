#include <colaDeRecursos.h>
#include <estructuras.h>
#include <recursos.h>
#include <planificacion.h>
#include <utils.h>

t_list* listaRecursos;

void iniciarListaDeRecursos(void) {
  t_list* listaRecursosDisponibles = recursosKernel->configuracion->RECURSOS;
  t_list* listaCantidadInstancias = recursosKernel->configuracion->INSTANCIAS_RECURSOS;
  listaRecursos = list_create();

  int cantRecursos = listaRecursosDisponibles->elements_count;
  for (int i = 0; i < cantRecursos; i++) {
    recursoSolicitados* recurso = malloc(sizeof(recursoSolicitados));
    recurso->recurso = list_get(listaRecursosDisponibles, i);
    recurso->cantidad_inst_recurso = list_get(listaCantidadInstancias, i);
    recurso->colaBloqueados = queue_create();
    list_add(listaRecursos, recurso);
  }
}

t_queue* crearColaRecursosBloqueados(void) {
  t_queue* colaRecursosBloqueados = queue_create();// creo la lista de recursos
  return colaRecursosBloqueados;
}

t_queue* devuelvoColaBloqueados(colaRecBloqueados) {
  return colaRecBloqueados;
}

int validarRecurso(char* recursoPedido) {
  int cantidadRecursos = listaRecursos->elements_count;// obtengo la cantidad de recursos
  int existeRecurso = 0;

  for (int i = 0; i < cantidadRecursos; i++) { // recorro la lista y valida que exsista el recurso pedido en la lista de recursos
    recursoSolicitados* reg_recurso =  list_get(listaRecursos, i);
    char* elementoRecurso = reg_recurso->recurso;

    if (strcmp(recursoPedido, elementoRecurso) == 0) {
      puts("Existe El recurso");
      existeRecurso = 1;
    }
  }
  return existeRecurso;
}

void procesarRecursoExistenteWait(recursoSolicitados* registroRecurso,t_queue* colaBloqueados) {
  int cantidadInstancias = registroRecurso->cantidad_inst_recurso;
  if(cantidadInstancias > 0){
    registroRecurso->cantidad_inst_recurso = registroRecurso->cantidad_inst_recurso - 1; //resto 1 a la cantidad de recursos disponibles
    //devuelvoColaBloqueados(colaBloqueados);
  }
  else {
    queue_push(colaBloqueados,registroRecurso); //bloqueo el recurso solicitado
    registroRecurso->colaBloqueados = colaBloqueados;
    //devuelvoColaBloqueados(colaBloqueados);
  }
}

void procesarRecursoSignal(char* recpedido) {
  t_list* listaRecursos;
  validoExistenciaDeRecursoSignal(listaRecursos,recpedido);
}

void validoExistenciaDeRecursoSignal(t_list* listaRecursos,char* recursopedido) {
  int cantRecursos = list_size(listaRecursos);// obtengo la cantidad de recursos
  int existeRecurso = 0;
  recursoSolicitados* reg_recurso;

  for (size_t i = 0; i < cantRecursos; i++) { // recorro la lista y valida que exsista el recurso pedido en la lista de recursos
    // Obtener el elemento en el frente de la cola
    reg_recurso =  list_get(listaRecursos, i);
    char* elementoRecurso = reg_recurso->recurso;

    if (recursopedido == elementoRecurso){
      existeRecurso = 1;
      t_queue* colaBloqueados;
      procesarRecursoExistenteSignal(reg_recurso,colaBloqueados);
    }
  }

  if(existeRecurso == 0) { //sino existe recurso lo mando al EXIT
    PCB* procesoTerminado = procesoEjecutandose;
    sacarDeEjecutando(EXIT);
    log_info(recursosKernel->logger,  "Finaliza el proceso, Motivo: No existe el recurso solicitado");
    finalizarProceso(procesoTerminado, INVALID_RESOURCE);
    liberarPcb(procesoTerminado);
  }
}

void procesarRecursoExistenteSignal(recursoSolicitados* registroRecurso,t_queue* colaBloqueados) {
  registroRecurso->cantidad_inst_recurso = registroRecurso->cantidad_inst_recurso + 1;//sumo 1 a la cantidad de recursos disponibles
  queue_pop(registroRecurso);// desbloqueo el primer elemento de la cola de bloqeados d ese recurso.
  // se devuelve la ejecución al proceso que peticionó el SIGNAL.
}
