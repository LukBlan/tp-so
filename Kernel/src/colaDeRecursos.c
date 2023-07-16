#include <colaDeRecursos.h>
#include <estructuras.h>
#include <recursos.h>
#include <planificacion.h>

t_list* cargarListaDeRecursos(void){
  t_list* listaRecursos = list_create();

  t_list* recursosDeLaConfig = recursosKernel->configuracion->RECURSOS;

  int cantRecursos = list_size(recursosDeLaConfig);
  recursoSolicitados* recurso;

  for (size_t i = 0; i < cantRecursos; i++) {

    char* auxRecursos = list_get(recursosDeLaConfig, i);
    int auxInstancias = list_get(recursosKernel->configuracion->INSTANCIAS_RECURSOS, i);

    recurso->recurso = auxRecursos;
    recurso->cantidad_inst_recurso = auxInstancias;

    list_add_in_index(listaRecursos, i, recurso);

    i+=1;
  }
  return listaRecursos;
}

t_queue* crearColaRecursosBloqueados(void){
  t_queue* colaRecursosBloqueados = queue_create();// creo la lista de recursos
  return colaRecursosBloqueados;
}

t_queue* devuelvoColaBloqueados(colaRecBloqueados){
  return colaRecBloqueados;
}

void validoExistenciaDeRecursoWait(t_list* listaRecursos,char* recursopedido) {
  int cantRecursos = list_size(listaRecursos);// obtengo la cantidad de recursos
  int existeRecurso = 0;
  recursoSolicitados* reg_recurso;

  for (size_t i = 0; i < cantRecursos; i++) { // recorro la lista y valida que exsista el recurso pedido en la lista de recursos
    reg_recurso =  list_get(listaRecursos, i);
    char* elementoRecurso = reg_recurso->recurso;
    int comparacion = strcmp(recursopedido, elementoRecurso);

    if (comparacion == 0){
      existeRecurso = 1;
      t_queue* colaBloqueados = crearColaRecursosBloqueados();
      procesarRecursoExistenteWait(reg_recurso,colaBloqueados);
    }
  }

  if(existeRecurso == 0){ //sino existe recurso lo mando al EXIT
    PCB* procesoTerminado = procesoEjecutandose;
    sacarDeEjecutando();
    log_info(recursosKernel->logger,  "Finaliza el proceso, Motivo: No existe el recurso solicitado");
    finalizarProceso(procesoTerminado, INVALID_RESOURCE);
    liberarPcb(procesoTerminado);
  }
}

void procesarRecursoWait(char* recursopedido){
  t_list* listRecursos = cargarListaDeRecursos();
  validoExistenciaDeRecursoWait(listRecursos, recursopedido);
}

void procesarRecursoExistenteWait(recursoSolicitados* registroRecurso,t_queue* colaBloqueados){

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

void procesarRecursoSignal(char* recpedido){
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

  if(existeRecurso == 0){ //sino existe recurso lo mando al EXIT
    PCB* procesoTerminado = procesoEjecutandose;
    sacarDeEjecutando();
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
