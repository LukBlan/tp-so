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
  int continuarEjecutando = 0;

  log_info(logger, "Ejecutando %s %s", identificador, primerParametro);
  if (strcmp("I/O", identificador) == 0) {
    int tiempoBloqueado = atoi(primerParametro);
    enviarContexto(contexto, socketKernel, IO);
    enviarEntero(tiempoBloqueado, socketKernel);
  } else if (strcmp("WAIT", identificador) == 0){
    char* recurso = primerParametro;
    enviarContexto(contexto, socketKernel, WAIT);
    enviarString(recurso, socketKernel);
  } else if (strcmp("SIGNAL", identificador) == 0){
    char* recurso = primerParametro;
    enviarContexto(contexto, socketKernel, SIGNAL);
    enviarString(recurso, socketKernel);
  } else if (strcmp("DELETE_SEGMENT", identificador) == 0) {
    int idSegmento = atoi(primerParametro);
    enviarContexto(contexto, socketKernel, DELETE_SEGMENT);
    enviarEntero(idSegmento, socketKernel);
  } else if (strcmp("F_OPEN", identificador) == 0) {
    char* nombreArchivo = primerParametro;
    enviarContexto(contexto, socketKernel, F_OPEN);
    enviarString(nombreArchivo,socketKernel);
  } else if (strcmp("F_CLOSE", identificador) == 0) {
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

char* valorRegistro(char* primerParametro ) {
  char* segundoParametro;
  if (strcmp("AX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.AX);
  } else if (strcmp("BX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.BX);
  } else if (strcmp("CX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.CX);
  } else if (strcmp("DX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.DX);
  } else if (strcmp("EAX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.EAX);
  } else if (strcmp("EBX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.EBX);
  } else if (strcmp("ECX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.ECX);
  } else if (strcmp("EDX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.EDX);
  } else if (strcmp("RAX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.RAX);
  } else if (strcmp("RBX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.RBX);
  } else if (strcmp("RCX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.RCX);
  } else if (strcmp("RDX", primerParametro) == 0) {
    strcpy(segundoParametro, recursosCpu->registros.RDX);
  }
  return segundoParametro;
}

int ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
  char* identificador = instruccion->strings[0];
  char* primerParametro = instruccion->strings[1];
  char* segundoParametro = instruccion->strings[2];
  int continuarEjecutando = 1;
  int socketKernel = recursosCpu->conexiones->socketKernel;
  int socketMemoria = recursosCpu->conexiones->socketMemoria;

  log_info(logger, "Ejecutando %s %s %s", identificador, primerParametro, segundoParametro);
  if (strcmp("SET", identificador) == 0) {
    //TODO Retardo de instruccion
    setearRegistro(primerParametro, segundoParametro);
    contexto->registros = recursosCpu->registros;
  } else if (strcmp("MOV_IN", identificador) == 0) {
    /*int direccionLogica = atoi(segundoParametro);
    char* registro = primerParametro;
    //comprobacion MMU
    enviarContexto(contexto, socketMemoria, MOV_IN);
    enviarEntero(direccionLogica,socketMemoria);
    char* parametro = recibirString(socketMemoria);
    setearRegistro(primerParametro,parametro);
    contexto->registros = recursosCpu->registros;
    */
	  continuarEjecutando = 0;
	  enviarContexto(contexto, socketKernel, MOV_IN);
  } else if (strcmp("MOV_OUT", identificador) == 0) {
    /*int direccionLogica = atoi (primerParametro);
    //comprobacion MMU
    char* registro = segundoParametro;
    char* valorDeRegistro = valorRegistro(segundoParametro);
    enviarContexto(contexto, socketMemoria, MOV_OUT);
    enviarEntero(direccionLogica,socketMemoria);
    enviarString(valorDeRegistro,socketMemoria);
    */
	  continuarEjecutando = 0;
	  enviarContexto(contexto, socketKernel, MOV_OUT);
  } else if (strcmp("F_SEEK", identificador) == 0) {
    int posicion = atoi(segundoParametro);
    char* nombreArchivo = primerParametro;
    enviarContexto(contexto, socketKernel, F_SEEK);
    enviarString(nombreArchivo,socketKernel);
    enviarEntero(posicion,socketKernel);
    /*
    op_code respuestaKernel = obtenerCodigoOperacion(socketKernel);
    switch(respuestaKernel) {
        case SUCCESS:
          contextoEjecucion* nuevoActualizado = recibirContexto(socketKernel);
          contexto = nuevoActualizado;
          break;
        default:
          puts("Como carajo llegue al default");
          break;
      }
   */
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, F_TRUNCATE);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    int idSegmento = atoi(primerParametro);
    int tamainoSegmento = atoi(segundoParametro);
    continuarEjecutando = 0;
    enviarContexto(contexto, socketKernel, CREATE_SEGMENT);
    enviarEntero(idSegmento, socketKernel);
    enviarEntero(tamainoSegmento, socketKernel);
  }
  return continuarEjecutando;
}

int ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instruccion) {
  t_log* logger = recursosCpu->logger;
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
