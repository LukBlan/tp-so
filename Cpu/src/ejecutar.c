#include <ejecutar.h>
#include <conexiones.h>
#include <estructuras.h>
#include <recursos.h>
#include <utils.h>

void ejecutarContexto(contextoEjecucion* contexto) {
  int continuarEjecutando = 1;

  while(continuarEjecutando) {
    mostrarContexto(contexto);
      t_instruccion* instruccion = list_get(contexto->instrucciones, contexto->programCounter);
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
    printf("Nombre Archivo %s\n", primerParametro);
    char* nombreArchivo = primerParametro;
    printf("Nombre Archivo %s\n", nombreArchivo);
    enviarContexto(contexto, socketKernel, F_OPEN);
    enviarString(nombreArchivo, socketKernel);
    op_code respuestaFS = obtenerCodigoOperacion(socketKernel);

    switch(respuestaFS) {
      case SUCCESS:
        puts("Entre en el switch");
        liberarContexto(contexto);
        puts("1");
        puts("2");
        contexto = recibirContexto(socketKernel);
        puts("3");
        continuarEjecutando = 1;
        break;
      case BLOCK:
         // DO NOTHING
        break;
      default:
        puts("Entre por default en F_Open");
        break;
    }

  } else if (strcmp("F_CLOSE", identificador) == 0) {
    enviarContexto(contexto, socketKernel, F_CLOSE);
    enviarString(primerParametro, socketKernel);
    contexto = recibirContexto(socketKernel);
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

int tamanioRegistro(char* primerParametro ) {
  if (strcmp("AX", primerParametro) == 0 ||strcmp("BX", primerParametro) == 0 ||
 strcmp("CX", primerParametro) == 0 || strcmp("DX", primerParametro) == 0  ) {
    return 4;
  } else if (strcmp("EAX", primerParametro) == 0 || strcmp("EBX", primerParametro) == 0 || 
  strcmp("ECX", primerParametro) == 0 || strcmp("EDX", primerParametro) == 0) {
    return 8;
  } else if (strcmp("RAX", primerParametro) == 0 || strcmp("RBX", primerParametro) == 0 || 
  strcmp("RCX", primerParametro) == 0 || strcmp("RDX", primerParametro) == 0) {
    return 16;
  }
}
Segmento* buscarSegmentoPorId(t_list* listaDeSegmentos,int id){
  Segmento* seg;
  for(int i = 0, i>list_size(listaDeSegmentos),i++){
    if(listaDeSegmentos[i]->id == id){
      seg = &listaDeSegmentos[i];
      return seg;
    }
  }
}
int posicionEnMemoria(int numeroSegmento,int numeroDesplazamiento,contextoEjecucion* contexto){
  t_lits* listaDeSegmento = contexto -> tablaSegmentos;
  Segmento* segmentoEncontrado = buscarSegmentoPorId(numeroSegmento);
  int baseSegmento = segmentoEncontrado -> base;
  return baseSegmento + numeroDesplazamiento ;
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
    /*int direccionLogica = atoi (segundoParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    char* registro = primerParametro;
    int tamanioALeer = darTamanioSegunRegistro(registro);
    if(numeroDesplazamiento + tamanioALeer > recursosCpu->configuracion->TAM_MAX_SEGMENTO){
      enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);
    }
    int posicion = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);
    char* registro = segundoParametro;
    enviarContexto(contexto, socketMemoria, MOV_OUT);
    enviarEntero(posicionEnMemoria,socketMemoria);
    enviarEntero(tamanioALeer,socketMemoria);
    op_code respuestaFS = obtenerCodigoOperacion(socketMemoria);
    switch(respuestaFS) {
                      case SUCCESS:
                        char* parametro = recibirString(socketMemoria);
                        setearRegistro(primerParametro,parametro);
                        contexto->registros = recursosCpu->registros;
                        break;
              }
  }
    */
	  continuarEjecutando = 1;
  } else if (strcmp("MOV_OUT", identificador) == 0) {
    /*int direccionLogica = atoi (primerParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    char* registro = segundoParametro;
    int tamanioALeer = darTamanioSegunRegistro(registro);
    if(numeroDesplazamiento + tamanioALeer > recursosCpu->configuracion->TAM_MAX_SEGMENTO){
      enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);
    }
    int posicion = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);
    char* registro = segundoParametro;
    char* valorDeRegistro = valorRegistro(segundoParametro);
    enviarContexto(contexto, socketMemoria, MOV_OUT);
    enviarEntero(posicionEnMemoria,socketMemoria);
    enviarString(valorDeRegistro,socketMemoria);
     op_code respuestaFS = obtenerCodigoOperacion(socketMemoria);
    switch(respuestaFS) {
                      case SUCCESS:
                        break;
              }
              
    */
	  continuarEjecutando = 1;
  } else if (strcmp("F_SEEK", identificador) == 0) {
    int posicion = atoi(segundoParametro);
    char* nombreArchivo = primerParametro;
    enviarContexto(contexto, socketKernel, F_SEEK);
    enviarString(nombreArchivo,socketKernel);
    enviarEntero(posicion,socketKernel);
    contexto = recibirContexto(socketKernel);
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    continuarEjecutando = 0;
    int tamanioNuevo = atoi(segundoParametro);
    char* nombreArchivoATruncar = primerParametro;
    enviarContexto(contexto, socketKernel, F_TRUNCATE);
    enviarString(nombreArchivoATruncar,socketKernel);
    enviarEntero(tamanioNuevo,socketKernel);
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
