#ifndef EJECUTAR_H
#define EJECUTAR_H

  #include <estructuras.h>

  #include <ejecutar.h>
#include <conexiones.h>
#include <estructuras.h>
#include <recursos.h>

void ejecutarContexto(contextoEjecucion* contexto);

int ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion);

int ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion);

int ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion);

int ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion);

int ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instruccion);


#endif
