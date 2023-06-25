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
  char* identificador = instruccion->strings[0];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s", identificador);
  if (strcmp("YIELD", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, YIELD);
  } else if (strcmp("EXIT", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, EXIT);
  }
  return continuarEjecutando;
}

int ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  int socketKernel = recursosCpu->conexiones->socketKernel;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  int continuarEjecutando;

  log_info(logger, "Ejecutando %s %s", identificador, primerParametro);
  if (strcmp("I/O", identificador) == 0) {
    int tiempoBloqueado = atoi(primerParametro);
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, IO);
    enviarEntero(tiempoBloqueado, socketKernel);
  } else if (strcmp("WAIT", identificador) == 0){
    char* recurso = primerParametro;
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, WAIT);
    enviarString(recurso, socketKernel);
  } else if (strcmp("SIGNAL", identificador) == 0){
    char* recurso = primerParametro;
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, SIGNAL);
    enviarString(recurso, socketKernel);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, CREATE_SEGMENT);
  } else if (strcmp("DELETE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, DELETE_SEGMENT);
  } else if (strcmp("F_OPEN", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, F_OPEN);
  } else if (strcmp("F_CLOSE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, F_CLOSE);
  }
    
  return continuarEjecutando;
}

void setearRegistro(char* primerParametro, char* segundoParametro) {

  if (strcmp("AX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.AX, segundoParametro);
  } else if (strcmp("BX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.BX, segundoParametro);
  } else if (strcmp("CX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.CX, segundoParametro);
  } else if (strcmp("DX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.DX, segundoParametro);
  } else if (strcmp("EAX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.EAX, segundoParametro);
  } else if (strcmp("EBX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.EBX, segundoParametro);
  } else if (strcmp("ECX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.ECX, segundoParametro);
  } else if (strcmp("EDX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.EDX, segundoParametro);
  } else if (strcmp("RAX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.RAX, segundoParametro);
  } else if (strcmp("RBX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.RBX, segundoParametro);
  } else if (strcmp("RCX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.RCX, segundoParametro);
  } else if (strcmp("RDX", primerParametro) == 0) {
    strcpy(recursosCpu->registros.RDX, segundoParametro);
  }
}

int ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];
  int continuarEjecutando = 1;

  log_info(logger, "Ejecutando %s %s %s", identificador, primerParametro, segundoParametro);
  if (strcmp("SET", identificador) == 0) {
    setearRegistro(primerParametro, segundoParametro);
    contexto->registros = recursosCpu->registros;
  } else if (strcmp("MOV_IN", identificador) == 0) {
  } else if (strcmp("MOV_OUT", identificador) == 0) {
  } else if (strcmp("F_SEEK", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_SEEK);
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_TRUNCATE);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, CREATE_SEGMENT);
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
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_READ);
  } else if (strcmp("F_WRITE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_WRITE);
  }

  return continuarEjecutando;
}
