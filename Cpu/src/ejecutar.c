#include <ejecutar.h>
#include <conexiones.h>
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
  //liberarContexto(contexto);
}

op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoPaquete;
  switch (instruccion->cantidadParametros) {
    case 0:
      codigoPaquete = ejecutarCeroParametros(contexto, instruccion, continuarEjecutando);
      break;
    case 1:
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
