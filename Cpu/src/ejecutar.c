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
      recursosCpu->registros = contexto->registros;
      codigoPaquete = ejecutarInstruccion(contexto, instruccion, continuarEjecutando);
      contexto->registros = recursosCpu->registros;
  }

  enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoPaquete);
  liberarContexto(contexto);
}

op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoPaquete = DESCONOCIDA;
  switch (instruccion->cantidadParametros) {
    case 0:
      codigoPaquete = ejecutarCeroParametros(contexto, instruccion, continuarEjecutando);
      break;
    case 1:
    	codigoPaquete = ejecutarUnParametro(contexto, instruccion, continuarEjecutando);
      break;
    case 2:
      codigoPaquete = ejecutarDosParametros(contexto, instruccion, continuarEjecutando);
      break;
    case 3:
      codigoPaquete = ejecutarTresParametros(contexto, instruccion, continuarEjecutando);
      break;
  }
  return codigoPaquete;
}

op_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];

  log_info(logger, "Ejecutando %s", identificador);
  if (strcmp("YIELD", identificador) == 0) {
    *continuarEjecutando = 0;
    codigoOperacion = YIELD;
  } else if (strcmp("EXIT", identificador) == 0) {
    *continuarEjecutando = 0;
    codigoOperacion = EXIT;
  }
  return codigoOperacion;
}

op_code ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];

  log_info(logger, "Ejecutando %s %s", identificador, primerParametro);
  if (strcmp("I/O", identificador) == 0) {
    int tiempoBloqueado = atoi(primerParametro);
    contexto->tiempoBloqueadoIO = tiempoBloqueado;
    codigoOperacion = IO;
  } else if (strcmp("WAIT", identificador) == 0){
    char* recurso = primerParametro;
    *continuarEjecutando = 0;
    codigoOperacion = WAIT;
  } else if (strcmp("SIGNAL", identificador) == 0){
    char* recurso = primerParametro;
    *continuarEjecutando = 0;
    codigoOperacion = SIGNAL;
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    *continuarEjecutando = 0;
    codigoOperacion = CREATE_SEGMENT;
  } else if (strcmp("DELETE_SEGMENT", identificador) == 0) {
    *continuarEjecutando = 0;
    codigoOperacion = DELETE_SEGMENT;
  }
    
  return codigoOperacion;  
}

op_code ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];

  log_info(logger, "Ejecutando %s %s %s", identificador, primerParametro, segundoParametro);
  if (strcmp("SET", identificador) == 0) {
    codigoOperacion = SET;
  } else if (strcmp("MOV_IN", identificador) == 0) {
  } else if (strcmp("MOV_OUT", identificador) == 0) {
  } else if (strcmp("F_SEEK", identificador) == 0) {
    *continuarEjecutando = 0;
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    *continuarEjecutando = 0;
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    *continuarEjecutando = 0;
  }
  return codigoOperacion;
}

op_code ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];
  char* tercerParametro = instruccion->strings[3];

  log_info(logger, "Ejecutando %s %s %s %s", identificador, primerParametro, segundoParametro, tercerParametro);
  if (strcmp("F_READ ", identificador) == 0) {
    codigoOperacion = SET;
    *continuarEjecutando = 0;
  } else if (strcmp("F_WRITE", identificador) == 0) {
    *continuarEjecutando = 0;
  }

  return codigoOperacion;
}
