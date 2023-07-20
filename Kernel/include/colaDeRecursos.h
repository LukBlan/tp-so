#ifndef COLA_DE_RECURSOS_H
#define COLA_DE_RECURSOS_H

#include <commons/collections/queue.h>

  typedef struct {
      char* recurso;
      int cantidad_inst_recurso;
      t_queue* colaBloqueados;
  } recursoSolicitados;

  extern t_list* listaRecursos;

  void iniciarListaDeRecursos(void);
  t_queue* crearColaRecursosBloqueados(void);
  t_queue* devuelvoColaBloqueados(colaRecBloqueados);
  void validoExistenciaDeRecursoWait(t_list* listaRecursos,char* recursopedido);
  void procesarRecursoWait(char* recursopedido);
  void procesarRecursoExistenteWait(recursoSolicitados* registroRecurso,t_queue* colaBloqueados);
  void procesarRecursoSignal(char* recpedido);
  void validoExistenciaDeRecursoSignal(t_list* listaRecursos,char* recursopedido);
  void procesarRecursoExistenteSignal(recursoSolicitados* registroRecurso,t_queue* colaBloqueados);
#endif
