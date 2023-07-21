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

int validarRecurso(char* recursoPedido) {
  int cantidadRecursos = listaRecursos->elements_count;// obtengo la cantidad de recursos
  int posicionRecurso = -1;

  for (int i = 0; i < cantidadRecursos; i++) { // recorro la lista y valida que exsista el recurso pedido en la lista de recursos
    recursoSolicitados* reg_recurso =  list_get(listaRecursos, i);
    char* elementoRecurso = reg_recurso->recurso;

    if (strcmp(recursoPedido, elementoRecurso) == 0) {
      puts("Existe El recurso");
      posicionRecurso = i;
    }
  }
  return posicionRecurso;
}

int validarInstanciasDeRecurso(int posicionRecurso) {
  recursoSolicitados* recurso =  list_get(listaRecursos, posicionRecurso);
  int cantidadInstancias = recurso->cantidad_inst_recurso;

  return (cantidadInstancias > 0)? 1 : 0;
}

void disminuirInstanciasRecurso(int posicionRecurso) {
  recursoSolicitados* recurso =  list_get(listaRecursos, posicionRecurso);
  recurso->cantidad_inst_recurso -= 1;
}

void bloquearProcesoPorRecurso(PCB* proceso, int posicionRecurso) {
  recursoSolicitados* recurso =  list_get(listaRecursos, posicionRecurso);
  queue_push(recurso->colaBloqueados, proceso);
  printf("Cantidad procesos bloqueados en recurso %s %d\n", recurso->recurso, recurso->colaBloqueados->elements->elements_count);
}

PCB* obtenerProcesoBloqueado(int posicionRecurso) {
  recursoSolicitados* recurso =  list_get(listaRecursos, posicionRecurso);
  printf("Cantidad procesos bloqueados en recurso %s %d\n", recurso->recurso, recurso->colaBloqueados->elements->elements_count);
  PCB* procesoBloqueado = queue_pop(recurso->colaBloqueados);
  return procesoBloqueado;
}

void aumentarRecurso(int posicionRecurso) {
  recursoSolicitados* recurso =  list_get(listaRecursos, posicionRecurso);
  recurso->cantidad_inst_recurso += 1;
}
