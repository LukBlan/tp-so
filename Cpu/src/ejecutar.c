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
    op_code respuestaKernel = obtenerCodigoOperacion(socketKernel);
    switch(respuestaKernel) {
      case SUCCESS:
        continuarEjecutando = 1;
        break;
      case BLOCK:
        liberarContexto(contexto);
        continuarEjecutando = 0;
         break;
      default:
        puts("Entre por default de wait");
        break;
    }
    contexto = recibirContexto(socketKernel);
  } else if (strcmp("SIGNAL", identificador) == 0){
    char* recurso = primerParametro;
    enviarContexto(contexto, socketKernel, SIGNAL);
    enviarString(recurso, socketKernel);
    op_code respuestaKer = obtenerCodigoOperacion(socketKernel);

    switch(respuestaKer) {
      case SUCCESS:
        continuarEjecutando = 1;
        break;
      case EXIT:
        continuarEjecutando = 0;
         break;
      default:
        puts("Entre por default de wait");
        break;
    }
    contexto = recibirContexto(socketKernel);
  } else if (strcmp("DELETE_SEGMENT", identificador) == 0) {
    continuarEjecutando = 1;
    int idSegmento = atoi(primerParametro);
    enviarContexto(contexto, socketKernel, DELETE_SEGMENT);
    enviarEntero(idSegmento, socketKernel);
    liberarContexto(contexto);
    obtenerCodigoOperacion(socketKernel);
    contexto = recibirContexto(socketKernel);
  } else if (strcmp("F_OPEN", identificador) == 0) {
    printf("Nombre Archivo %s\n", primerParametro);
    char* nombreArchivo = primerParametro;
    printf("Nombre Archivo %s\n", nombreArchivo);
    enviarContexto(contexto, socketKernel, F_OPEN);
    enviarString(nombreArchivo, socketKernel);
    op_code respuestaFS = obtenerCodigoOperacion(socketKernel);

    switch(respuestaFS) {
      case SUCCESS_OPEN_CPU:
        puts("Entre en el switch");
        liberarContexto(contexto);
        puts("1");
        puts("2");
        contexto = recibirContexto(socketKernel);
        puts("3");
        continuarEjecutando = 1;
        break;
      case BLOCK:
        contextoEjecucion* contextoAlPedo = recibirContexto(socketKernel);
        liberarContexto(contextoAlPedo);
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
    memcpy(recursosCpu->registros.AX, segundoParametro, 4);
  } else if (strcmp("BX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.BX, segundoParametro, 4);
  } else if (strcmp("CX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.CX, segundoParametro, 4);
  } else if (strcmp("DX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.DX, segundoParametro, 4);
  } else if (strcmp("EAX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.EAX, segundoParametro, 8);
  } else if (strcmp("EBX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.EBX, segundoParametro, 8);
  } else if (strcmp("ECX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.ECX, segundoParametro, 8);
  } else if (strcmp("EDX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.EDX, segundoParametro, 8);
  } else if (strcmp("RAX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.RAX, segundoParametro, 16);
  } else if (strcmp("RBX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.RBX, segundoParametro, 16);
  } else if (strcmp("RCX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.RCX, segundoParametro, 16);
  } else if (strcmp("RDX", primerParametro) == 0) {
    memcpy(recursosCpu->registros.RDX, segundoParametro, 16);
  }
}

char* valorRegistro(char* primerParametro, int tamanioALeer) {
  char* segundoParametro = malloc(tamanioALeer + 1);

  if (strcmp("AX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.AX, tamanioALeer);
  } else if (strcmp("BX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.BX, tamanioALeer);
  } else if (strcmp("CX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.CX, tamanioALeer);
  } else if (strcmp("DX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.DX, tamanioALeer);
  } else if (strcmp("EAX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.EAX, tamanioALeer);
  } else if (strcmp("EBX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.EBX, tamanioALeer);
  } else if (strcmp("ECX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.ECX, tamanioALeer);
  } else if (strcmp("EDX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.EDX, tamanioALeer);
  } else if (strcmp("RAX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.RAX, tamanioALeer);
  } else if (strcmp("RBX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.RBX, tamanioALeer);
  } else if (strcmp("RCX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.RCX, tamanioALeer);
  } else if (strcmp("RDX", primerParametro) == 0) {
    memcpy(segundoParametro, recursosCpu->registros.RDX, tamanioALeer);
  }

  segundoParametro[tamanioALeer] = '\0';
  return segundoParametro;
}

int tamanioRegistro(char* primerParametro ) {
  int tamanioTotal;

  if (strlen(primerParametro) == 2) {
    tamanioTotal = 4;
  } else if (primerParametro[0] == 'E') {
    tamanioTotal = 8;
  } else if (primerParametro[0] == 'R') {
    tamanioTotal = 16;
  }

  return tamanioTotal;
}

Segmento* buscarSegmentoPorId(t_list* listaDeSegmentos, int idIngresada) {
  int cantidadSegmentos = listaDeSegmentos->elements_count;
  int posicion = -1;

  for(int i = 0; i < cantidadSegmentos; i++) {

	  Segmento* segmentoEncontrado  = list_get(listaDeSegmentos,i);
	  if(segmentoEncontrado->id == idIngresada){
	    posicion = i;
    }
  }
  return list_get(listaDeSegmentos, posicion);
}

int posicionEnMemoria(int numeroSegmento, int numeroDesplazamiento, contextoEjecucion* contexto){
  t_list* listaDeSegmento = contexto -> tablaSegmentos;
  Segmento* segmentoEncontrado = buscarSegmentoPorId(listaDeSegmento, numeroSegmento);
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
    int retardoInstruccion = recursosCpu->configuracion->RETARDO_INSTRUCCION * 1000;
    usleep(retardoInstruccion);
    setearRegistro(primerParametro, segundoParametro);
    contexto->registros = recursosCpu->registros;
    mostrarRegistros(contexto->registros);
  } else if (strcmp("MOV_IN", identificador) == 0) {
    int direccionLogica = atoi (segundoParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    char* registro = primerParametro;
    int tamanioALeer = tamanioRegistro(registro);
    Segmento* segmento = buscarSegmentoPorId(contexto->tablaSegmentos, numeroSegmento);

    if(numeroDesplazamiento + tamanioALeer > segmento->limite) {
      enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);
      continuarEjecutando = 0;
    } else {
      int posicion = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);

      enviarContexto(contexto, socketMemoria, MOV_IN);
      enviarEntero(posicion,socketMemoria);
      enviarEntero(tamanioALeer,socketMemoria);

      char* parametro = recibirString(socketMemoria);
      printf("El parametro recibido es %s\n", parametro);
      setearRegistro(primerParametro,parametro);
      contexto->registros = recursosCpu->registros;
      continuarEjecutando = 1;
    }
  } else if (strcmp("MOV_OUT", identificador) == 0) {
    int direccionLogica = atoi(primerParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    Segmento* segmento = buscarSegmentoPorId(contexto->tablaSegmentos, numeroSegmento);
    char* registro = segundoParametro;
    int tamanioALeer = tamanioRegistro(registro);

    // Si hay problemas chqueamos aca
    if(numeroDesplazamiento + tamanioALeer > segmento->limite){
      enviarContexto(contexto, socketKernel, SEGMENTATION_FAULT);
      continuarEjecutando = 0;
    } else {
      int posicion2 = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);

      char* valorDeRegistro = valorRegistro(segundoParametro, tamanioALeer);
      enviarContexto(contexto, socketMemoria, MOV_OUT);
      enviarEntero(posicion2,socketMemoria);
      enviarString(valorDeRegistro,socketMemoria);
      obtenerCodigoOperacion(socketMemoria);
      contextoEjecucion* contextoAlPedo = recibirContexto(socketMemoria);
      continuarEjecutando = 1;
    }


  } else if (strcmp("F_SEEK", identificador) == 0) {
    int posicion = atoi(segundoParametro);
    char* nombreArchivo = primerParametro;
    enviarContexto(contexto, socketKernel, F_SEEK);
    enviarString(nombreArchivo,socketKernel);
    enviarEntero(posicion,socketKernel);
    liberarContexto(contexto);
    obtenerCodigoOperacion(socketKernel);
    contexto = recibirContexto(socketKernel);
  } else if (strcmp("F_TRUNCATE", identificador) == 0) {
    continuarEjecutando = 0;
    int tamanioNuevo = atoi(segundoParametro);
    printf("Tamanio de truncate %d", tamanioNuevo);
    char* nombreArchivoATruncar = primerParametro;
    enviarContexto(contexto, socketKernel, F_TRUNCATE);
    enviarString(nombreArchivoATruncar,socketKernel);
    enviarEntero(tamanioNuevo,socketKernel);
  } else if (strcmp("CREATE_SEGMENT", identificador) == 0) {
    int idSegmento = atoi(primerParametro);
    int tamainoSegmento = atoi(segundoParametro);
    continuarEjecutando = 1;
    enviarContexto(contexto, socketKernel, CREATE_SEGMENT);
    enviarEntero(idSegmento, socketKernel);
    enviarEntero(tamainoSegmento, socketKernel);

    liberarContexto(contexto);
    op_code respuestaKernelCreacion = obtenerCodigoOperacion(socketKernel);
    contexto = recibirContexto(socketKernel);
    switch(respuestaKernelCreacion) {
      case SUCCESS:
        continuarEjecutando = 1;
        break;
      case OUT_OF_MEMORY:
        continuarEjecutando = 0;
        break;
      default:
        puts("Entre por el default de create_segment");
        break;
    }
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
  int socketKernel = recursosCpu->conexiones->socketKernel;

  log_info(logger, "Ejecutando %s %s %s %s", identificador, primerParametro, segundoParametro, tercerParametro);
  if (strcmp("F_READ", identificador) == 0) {
    continuarEjecutando = 0;
    char* nombreArchivoAEscribir = primerParametro;
    int direccionLogica = atoi(segundoParametro);
    int tamanio = atoi(tercerParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    if(numeroDesplazamiento + tamanio > recursosCpu->configuracion->TAM_MAX_SEGMENTO){
      enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);
      return continuarEjecutando;
    }
     int posicion = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_READ);
    enviarString(nombreArchivoAEscribir,socketKernel);
    enviarEntero(posicion,socketKernel);
    enviarEntero(tamanio,socketKernel);
  } else if (strcmp("F_WRITE", identificador) == 0) {
    continuarEjecutando = 0;
    char* nombreArchivoAEscribir = primerParametro;
    int direccionLogica = atoi(segundoParametro);
    int tamanio = atoi(tercerParametro);
    int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);
    int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);
    if(numeroDesplazamiento + tamanio > recursosCpu->configuracion->TAM_MAX_SEGMENTO){
      enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);
      return continuarEjecutando;
    }
    int posicion = posicionEnMemoria(numeroSegmento,numeroDesplazamiento,contexto);
    enviarContexto(contexto, recursosCpu->conexiones->socketKernel, F_WRITE);
    enviarString(nombreArchivoAEscribir,socketKernel);
    enviarEntero(posicion,socketKernel);
    enviarEntero(tamanio,socketKernel);
  }

  return continuarEjecutando;
}
