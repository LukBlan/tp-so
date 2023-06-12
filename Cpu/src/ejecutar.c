#include <ejecutar.h>
#include <conexiones.h>
#include <recursos.h>

void ejecutarContexto(contextoEjecucion* contexto) {
  t_list* listaInstrucciones = contexto->instrucciones;
  op_code codigoPaquete;
  int* continuarEjecutando = malloc(sizeof(int));

  for (int i = contexto->programCounter; i < list_size(listaInstrucciones); i++) {
      t_instruccion* instruccion = list_get(listaInstrucciones, contexto->programCounter);
      contexto->programCounter++;
      codigoPaquete = ejecutarInstruccion(contexto, instruccion, continuarEjecutando);     
  }
  //liberarContexto(contexto);
}

op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoPaquete;
  switch (instruccion->cantidadParametros) {
    case 0:
      ejecutarCeroParametros(contexto, instruccion);
      break;
    case 1:
    //codigoPaquete = ejecutarUnParametro(contexto, instruccion, continuarEjecutando);
      break;
    case 2:
      break;
    case 3:
      break;
  }
  return codigoPaquete;
}

void ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  char* identificador = instruccion->strings[0];
  t_log* logger = recursosCpu->logger;

  if (strcmp("YIELD", identificador) == 0) {
    log_info(logger, "Ejecutando YIELD");
    codigoOperacion = YIELD;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("EXIT", identificador) == 0) {
    log_info(logger, "Ejecutando EXIT");
    codigoOperacion = EXIT;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  }
}
/*
op_code ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando){
  op_code codigoOperacion;
  char* identificador = instruccion->strings[0];
  t_log* logger = recursosCpu->logger;

  if (strcmp("I/O", identificador) == 0) {
    log_info(logger, "Ejecutando I/O");
    int tiempoBloqueado = instruccion->strings[1];
    contexto->tiempoBloqueadoIO = tiempoBloqueado ;
    codigoOperacion = IO;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
    } else if (strcmp("WAIT", identificador) == 0){
      log_info(logger, "Ejecutando WAIT");

    } else if (strcmp("SIGNAL", identificador) == 0){
      log_info(logger, "Ejecutando SIGNAL");
    }
    
  return codigoOperacion;  
}