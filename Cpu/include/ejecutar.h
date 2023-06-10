#ifndef EJECUTAR_H
#define EJECUTAR_H

  #include <estructuras.h>

  void ejecutarContexto(contextoEjecucion* contexto);
  op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando);
  op_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando);

#endif
