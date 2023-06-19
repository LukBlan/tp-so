#include <ejecutar.h>
#include <conexiones.h>
#include <estructuras.h>
#include <recursos.h>

void ejecutarContexto(contextoEjecucion* contexto) {
  t_list* listaInstrucciones = contexto->instrucciones;
  int continuarEjecutando = 1;


  while(continuarEjecutando) {
      t_instruccion* instruccion = list_get(listaInstrucciones, contexto->programCounter);
      contexto->programCounter++;
      recursosCpu->registros = contexto->registros;
      continuarEjecutando = ejecutarInstruccion(contexto, instruccion);
      contexto->registros = recursosCpu->registros;
  }
  liberarContexto(contexto);
}

int ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion) {
  int continuarEjecutando;

  switch (instruccion->cantidadParametros) {
    case 0:
      continuarEjecutando = ejecutarCeroParametros(contexto, instruccion);
      break;
    case 1:
      continuarEjecutando = ejecutarUnParametro(contexto, instruccion);
      break;
    case 2:
      continuarEjecutando = ejecutarDosParametros(contexto, instruccion);
      break;
    case 3:
      continuarEjecutando = ejecutarTresParametros(contexto, instruccion);
      break;
  }
  return continuarEjecutando;
}

op_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s", identificador);
  if (strcmp("YIELD", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = YIELD;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("EXIT", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = EXIT;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  }
  return continuarEjecutando;
}

int ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s %s", identificador, primerParametro);
  if (strcmp("I/O", identificador) == 0) {
    int tiempoBloqueado = atoi(primerParametro);
    contexto->tiempoBloqueadoIO = tiempoBloqueado;
    codigoOperacion = IO;
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("WAIT", identificador) == 0){
    char* recurso = primerParametro;
    continuarEjecutando = 0;
    codigoOperacion = WAIT;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("SIGNAL", identificador) == 0){
    char* recurso = primerParametro;
    continuarEjecutando = 0;
    codigoOperacion = SIGNAL;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = CREATE_SEGMENT;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("DELETE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = DELETE_SEGMENT;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("F_OPEN", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = F_OPEN;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("F_CLOSE", identificador) == 0) {
    continuarEjecutando = 0;
    codigoOperacion = F_CLOSE;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  }
    
  return continuarEjecutando;
}

int ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s %s %s", identificador, primerParametro, segundoParametro);
  if (strcmp("SET", identificador) == 0) {
    codigoOperacion = SET;
  } else if (strcmp("MOV_IN", identificador) == 0) {
  } else if (strcmp("MOV_OUT", identificador) == 0) {
  } else if (strcmp("F_SEEK", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  }
  return continuarEjecutando;
}

int ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  op_code codigoOperacion = DESCONOCIDA;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];
  char* tercerParametro = instruccion->strings[3];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s %s %s %s", identificador, primerParametro, segundoParametro, tercerParametro);
  if (strcmp("F_READ", identificador) == 0) {
    codigoOperacion = F_READ;
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  } else if (strcmp("F_WRITE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoOperacion);
  }

  return continuarEjecutando;
}
