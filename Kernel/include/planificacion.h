#ifndef PROCESO_H_
#define PROCESO_H_

  #include <stdbool.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <string.h>

  #include <commons/log.h>

  #include <estructuras.h>
  #include <stdbool.h>
  
  extern t_list* tablaGlobalDeArchivos;
  void comenzarPlanificadores();
  void planificador_corto_plazo_fifo();
  void planificador_largo_plazo();
  void agregarAListo(PCB* proceso);
  int sePuedeAgregarMasProcesos();
  void cambiarEstado(estadoProceso estado, PCB* proceso);
  void ejecutar(PCB* proceso);
  void finalizarProceso(PCB* procesoFinalizado, op_code motivo);

#endif
