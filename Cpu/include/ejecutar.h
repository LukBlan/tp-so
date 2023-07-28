#ifndef EJECUTAR_H
#define EJECUTAR_H

  #include <estructuras.h>
  #include <ejecutar.h>
  #include <conexiones.h>
  #include <estructuras.h>
  #include <recursos.h>

<<<<<<< HEAD
  extern int idProcesoEjecutandose;
=======
extern int idProcesoEjecutandose;
>>>>>>> a2b1946aedc9a27fb9c0c7401f0b13b0623f1b0a

  void ejecutarContexto(contextoEjecucion* contexto);
  int ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion);
  int ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion);
  int ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion);
  int ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion);
  int ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instruccion);
  void setearRegistro(char* primerParametro, char* segundoParametro);
  char* valorRegistro(char* primerParametro, int tamanioALeer);
#endif
