#ifndef COLA_DE_RECURSOS_H
#define COLA_DE_RECURSOS_H

#include <commons/collections/queue.h>

  typedef struct {
      char* recurso;
      int cantidad_inst_recurso;
      t_queue* colaBloqueados;
  } recursoSolicitados;

#endif
