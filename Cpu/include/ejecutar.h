#ifndef EJECUTAR_H
#define EJECUTAR_H

  #include <estructuras.h>
  #include <ejecutar.h>
  #include <conexiones.h>
  #include <estructuras.h>
  #include <recursos.h>

  extern int idProcesoEjecutandose;
  extern contextoEjecucion* contexto;

  void ejecutarContexto();
  int ejecutarInstruccion(t_instruccion* instruccion);
  int ejecutarCeroParametros(t_instruccion* instruccion);
  int ejecutarUnParametro(t_instruccion* instruccion);
  int ejecutarDosParametros(t_instruccion* instruccion);
  int ejecutarTresParametros(t_instruccion* instruccion);
  void setearRegistro(char* primerParametro, char* segundoParametro);
  char* valorRegistro(char* primerParametro, int tamanioALeer);

#endif
