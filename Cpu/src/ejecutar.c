#include <ejecutar.h>
#include <conexiones.h>
#include <estructuras.h>
#include <recursos.h>

void ejecutarContexto(contextoEjecucion* contexto) {
  t_list* listaInstrucciones = contexto->instrucciones;
  op_code codigoPaquete;
  int* continuarEjecutando = malloc(sizeof(int));
  *continuarEjecutando = 1;

  while(*continuarEjecutando) {
      t_instruccion* instruccion = list_get(listaInstrucciones, contexto->programCounter);
      contexto->programCounter++;
      codigoPaquete = ejecutarInstruccion(contexto, instruccion, continuarEjecutando);
  }
  enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoPaquete);
  liberarContexto(contexto);
}

op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoPaquete;
  switch (instruccion->cantidadParametros) {
    case 0:
      codigoPaquete = ejecutarCeroParametros(contexto, instruccion, continuarEjecutando);
      break;
    case 1:
    	codigoPaquete = ejecutarUnParametro(contexto, instruccion, continuarEjecutando);
      break;
    case 2:
      break;
    case 3:
      break;
  }
  return codigoPaquete;
}

op_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoOperacion;
  char* identificador = instruccion->strings[0];
  t_log* logger = recursosCpu->logger;

  if (strcmp("YIELD", identificador) == 0) {
    log_info(logger, "Ejecutando YIELD");
    *continuarEjecutando = 0;
    codigoOperacion = YIELD;
  } else if (strcmp("EXIT", identificador) == 0) {
    log_info(logger, "Ejecutando EXIT");
    *continuarEjecutando = 0;
    codigoOperacion = EXIT;
  }
  return codigoOperacion;
}

op_code ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando){
  op_code codigoOperacion;
  char* identificador = instruccion->strings[0];
  t_log* logger = recursosCpu->logger;

  if (strcmp("I/O", identificador) == 0) {
      log_info(logger, "Ejecutando I/O");
      int tiempoBloqueado = atoi(instruccion->strings[1]);
      //contexto->tiempoBloqueadoIO = tiempoBloqueado ;
      codigoOperacion = IO;
      enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
    } else if (strcmp("WAIT", identificador) == 0){
      char* recurso = instruccion->strings[1];
      log_info(logger, "Ejecutando WAIT: %c", *recurso);
      *continuarEjecutando = 0;
      codigoOperacion = WAIT;
    } else if (strcmp("SIGNAL", identificador) == 0){
      char* recurso = instruccion->strings[1];
      log_info(logger, "Ejecutando SIGNAL: %c", *recurso);
      *continuarEjecutando = 0;
      codigoOperacion = SIGNAL;
    }
    
  return codigoOperacion;  
}
