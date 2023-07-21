#ifndef COLA_DE_RECURSOS_H
#define COLA_DE_RECURSOS_H

  #include <commons/collections/queue.h>
  #include <estructuras.h>

  typedef struct {
      char* recurso;
      int cantidad_inst_recurso;
      t_queue* colaBloqueados;
  } recursoSolicitados;

  extern t_list* listaRecursos;

  void iniciarListaDeRecursos(void);
  int validarRecurso(char* recursoPedido);
  int validarInstanciasDeRecurso(int posicionRecurso);
  void disminuirInstanciasRecurso(int posicionRecurso);
  void bloquearProcesoPorRecurso(PCB* proceso, int posicionRecurso);
  PCB* obtenerProcesoBloqueado(int posicionRecurso);
  void aumentarRecurso(int posicionRecurso);

#endif
