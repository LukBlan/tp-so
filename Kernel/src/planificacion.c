#include <planificacion.h>
#include <estructuras.h>
#include <recursos.h>
#include <serializacion/contexto.h>
#include <serializacion/buffer.h>
#include <serializacion/paquete.h>
#include <conexiones.h>
#include <utils.h>
#include <colaDeRecursos.h>

t_list* tablaGlobalDeArchivos;
int cantidadDeProcesos = 0;
int usoDeMemoria = 0;

void planificador_corto_plazo_fifo() {
  t_log* logger = recursosKernel->logger;
  log_info(logger, "INICIO PLANIFICACION CORTO PLAZO FIFO");
  while (1) {
    sem_wait(&semProcesoReady);
    sem_wait(&semaforoCantidadProcesosExec);
    //TODO ver lista
    procesoEjecutandose = list_remove(colaReady, 0);
    ejecutar(procesoEjecutandose);
  }
}

int tiempoAhora() {
  return time(NULL);
}

int calcular_tiempo_rafaga_real_anterior(PCB *proceso) {
    return tiempoAhora() - proceso->llegadaReady;
}

float estimacion(PCB* proceso) {
  t_configuracion* config = recursosKernel->configuracion;
  float alfa = config->HRRN_ALFA;
  float estimacionAnterior = proceso->estimadoRafaga;
  int rafagaPrevia = proceso->rafagaRealPrevia ;
  float resultado = alfa*rafagaPrevia + (1-alfa) * estimacionAnterior;
  return resultado;
}

float calcularResponseRatio (PCB* proceso) {
  return ((tiempoAhora()-proceso->llegadaReady)+estimacion(proceso))/estimacion(proceso);
}

bool ordenarSegunCalculoHRRN(void* proceso1, void* proceso2) {
  return calcularResponseRatio((PCB*)proceso1) > calcularResponseRatio((PCB*)proceso2);
}

PCB* sacarProcesoMayorHRRN() {
  PCB* procesoAEjecutar;
  pthread_mutex_lock(&mutexColaReady);
  list_sort(colaReady, (void*)ordenarSegunCalculoHRRN);
  procesoAEjecutar = list_remove (colaReady,0);
  pthread_mutex_unlock(&mutexColaReady);
  return procesoAEjecutar;
}

void planificador_corto_plazo_HRRN() {
  t_log* logger = recursosKernel->logger;
  log_info(logger, "INICIO PLANIFICACION HRRN");
  while (1) {
    sem_wait(&semProcesoReady);
    sem_wait(&semaforoCantidadProcesosExec);

    PCB* procesoEjecutar = sacarProcesoMayorHRRN();

    cambiarEstado(EXEC, procesoEjecutar);

    ejecutar(procesoEjecutar);
  }
}

PCB* sacarBloqueado() {
  t_log* logger = recursosKernel->logger;
  PCB *pcbSaliente;

  pthread_mutex_lock(&mutexColaBlock);
  pcbSaliente = queue_pop(colaBlock);
  log_info(logger, "Proceso: [%d] salío de BLOQUEADO.", pcbSaliente->pid);
  pthread_mutex_unlock(&mutexColaBlock);

  return pcbSaliente;
}

void *io() {
  t_log* logger = recursosKernel->logger;
  while (1) {

  sem_wait(&blockCounter);
  PCB* proceso = queue_peek(colaBlock); // Aun sigue en la cola de bloqueado.
  //TODO campo PCB
  int tiempoBloqueo = proceso->tiempoBloqueadoIO;

  log_info(logger, "----------[DISP I/O] Proceso: [%d] ,se bloqueara %d segundos.----------", proceso->pid, tiempoBloqueo);
  sleep(tiempoBloqueo);
  log_info(logger, "----------[DISP I/O] Proceso: [%d] ,termino I/O %d segundos.----------", proceso->pid, tiempoBloqueo);

  proceso = sacarBloqueado();
  agregarAListo(proceso);
  }
}

void agregar_proceso_bloqueado(PCB *procesoBloqueado) {

    pthread_mutex_lock(&mutexColaBlock);

    queue_push(colaBlock, procesoBloqueado);
    log_info(recursosKernel->logger, "Proceso: [%d] se movio a BLOQUEADO.", procesoBloqueado->pid);

    pthread_mutex_unlock(&mutexColaBlock);

    // Despierto al planificador de largo plazo

    sem_post(&largoPlazo);
}
void agregar_proceso_bloqueado_io(PCB *procesoBloqueado) {

    pthread_mutex_lock(&mutexColaBlock);

    queue_push(colaBlock, procesoBloqueado);
    log_info(recursosKernel->logger, "Proceso: [%d] se movio a BLOQUEADO.", procesoBloqueado->pid);

    pthread_mutex_unlock(&mutexColaBlock);

    // Despierto dispositivo I/O
    sem_post(&blockCounter);

    // Despierto al planificador de largo plazo

    sem_post(&largoPlazo);
}

void comenzarPlanificadores() {
  pthread_t hilo_largo_plazo;
  pthread_t hiloCortoPlazo;
  pthread_t hilo_dispositivo_io;

  pthread_create(&hilo_largo_plazo, NULL, (void*)planificador_largo_plazo, NULL);
  pthread_detach(hilo_largo_plazo);

  if (strcmp(recursosKernel->configuracion->ALGORITMO_PLANIFICACION, "FIFO") == 0) {
    pthread_create(&hiloCortoPlazo, NULL, (void*)planificador_corto_plazo_fifo, NULL);
    pthread_detach(hiloCortoPlazo);
  }
  else
  {
    pthread_create(&hiloCortoPlazo, NULL, (void*)planificador_corto_plazo_HRRN, NULL);
    pthread_detach(hiloCortoPlazo);
  }

  pthread_create(&hilo_dispositivo_io, NULL, io, NULL);
  pthread_detach(hilo_dispositivo_io);

}

void planificador_largo_plazo() {
  t_log* logger = recursosKernel->logger;
  t_configuracion* config = recursosKernel->configuracion;

  log_info(logger, "Inicio PLanificador LARGO PLAZO en [%s]", config->ALGORITMO_PLANIFICACION);
  while (1) {
    sem_wait(&largoPlazo);
    if (sePuedeAgregarMasProcesos()) {
      PCB *procesoSaliente = queue_pop(colaNew);
      agregarAListo(procesoSaliente);
      cambiarEstado(READY, procesoSaliente);
    }
    log_info(logger, "[LARGO-PLAZO] Procesos en Memoria: %d", list_size(colaReady));
  }
}

int sePuedeAgregarMasProcesos() {
  t_configuracion* config = recursosKernel->configuracion;
  return (config->GRADO_MAX_MULTIPROGRAMACION >= cantidadDeProcesos && queue_size(colaNew) > 0)? 1 : 0;
}

char* asignarStringLiteral(char* stringLiteral) {
  int cantidadCaracteres = strlen(stringLiteral) + 1;
  char* stringAllocated = malloc(cantidadCaracteres);
  strcpy(stringAllocated, stringLiteral);
  stringAllocated[cantidadCaracteres - 1] = '\0';
  return stringAllocated;
}

char* estadoAsString(estadoProceso estado) {
  char* estadoAsString;
  switch(estado) {
    case NEW:
      estadoAsString = asignarStringLiteral("New");
      break;

    case READY:
      estadoAsString = asignarStringLiteral("Ready");
      break;

    case EXEC:
      estadoAsString = asignarStringLiteral("Execute");
      break;

    case BLOCK:
      estadoAsString = asignarStringLiteral("Block");
      break;

    case EXITSTATE:
      estadoAsString = asignarStringLiteral("Exit");
      break;

    default:
      estadoAsString = asignarStringLiteral("Sin Estado");
      break;
  }
  return estadoAsString;
}

void cambiarEstado(estadoProceso estado, PCB* proceso) {
  char* estadoAnterior = estadoAsString(proceso->estado);
  proceso->estado = estado;
  char* estadoNuevo = estadoAsString(proceso->estado);

  log_info(
      recursosKernel->logger,
      "Cambio de Estado: PID: [%d] - Estado Anterior: %s - Estado Actual: %s",
      proceso->pid, estadoAnterior, estadoNuevo
  );
  free(estadoAnterior);
  free(estadoNuevo);
}
char* obtenerProcesosEnReady() {
  int cantidadProcesEnReady = colaReady->elements_count;
  char* stringDeProcesos = malloc(cantidadProcesEnReady * 2);
  PCB* primerProceso = list_get(colaReady, 0);
  stringDeProcesos[0] = '0' + primerProceso->pid;

  for (int i = 1,j = 1; j < cantidadProcesEnReady; i+=2,j++) {
    PCB* proceso = list_get(colaReady, j);
    stringDeProcesos[i] = '-';
    stringDeProcesos[i+1] = '0' + proceso->pid;
  }

  stringDeProcesos[(cantidadProcesEnReady * 2) - 1] = '\0';
  return stringDeProcesos;
}

void agregarAListo(PCB* proceso) {
  t_log* logger = recursosKernel->logger;
  pthread_mutex_lock(&mutexColaReady);
  list_add(colaReady, proceso);
  log_info(logger, "Proceso: [%d] se movio a listo", proceso->pid);
  log_info(
    logger,
    "Cola Ready <%s>: <%s>",
    recursosKernel->configuracion->ALGORITMO_PLANIFICACION, obtenerProcesosEnReady()
  );
  pthread_mutex_unlock(&mutexColaReady);
  sem_post(&semProcesoReady);
}

void sacarDeEjecutando(estadoProceso estado,PCB* proceso) {
	  proceso->estimadoRafaga = estimacion(proceso);
	  proceso->rafagaRealPrevia = calcular_tiempo_rafaga_real_anterior(proceso);
	t_log* logger = recursosKernel->logger;

  pthread_mutex_lock(&mutexColaExec);
  log_info(logger, "Proceso: [%d] salío de EJECUTANDO.", procesoEjecutandose->pid);
  cambiarEstado(estado, procesoEjecutandose);
  procesoEjecutandose = NULL;
  pthread_mutex_unlock(&mutexColaExec);
  sem_post(&semaforoCantidadProcesosExec);
}

void actualizarContexto(PCB* proceso, contextoEjecucion* nuevoContexto) {
  liberarContexto(proceso->contexto);
  proceso->contexto = nuevoContexto;
}

int buscarSocket(int pidProceso) {
  t_list* procesosConsola = recursosKernel->conexiones->procesosConsola;
  int cantidadProcesosConsola = procesosConsola->elements_count;

  for (int i = 0; i < cantidadProcesosConsola; i++) {
    procesoConsola* proceso = list_get(procesosConsola, i);
    if (proceso->pid == pidProceso) {
      return i;
    }
  }
  return -1;
}

void terminarConsola(procesoConsola* consolaFinalizada, int posicionProceso) {
  list_remove(recursosKernel->conexiones->procesosConsola, posicionProceso);
  close(consolaFinalizada->socketConsola);
  free(consolaFinalizada);
}

void avisarProcesoFinalizado(int socket, op_code motivo) {
  t_buffer* buffer = generarBuffer(4);
  t_paquete* paquete = crearPaquete(buffer, motivo);
  enviar_paquete(paquete, socket);
  liberarPaquete(paquete);
}

void finalizarProceso(PCB* procesoFinalizado, op_code motivo) {
  int posicionProceso = buscarSocket(procesoFinalizado->pid);
  procesoConsola* consolaFinalizada = list_get(recursosKernel->conexiones->procesosConsola, posicionProceso);
  int socketConsola = consolaFinalizada->socketConsola;
  int socketMemoria = recursosKernel->conexiones->socketMemoria;

  cantidadDeProcesos--;
  avisarProcesoFinalizado(socketConsola, motivo);
  pthread_mutex_lock(&operandoConMemoria);
  avisarProcesoFinalizado(socketMemoria, motivo);
  enviarEntero(procesoFinalizado->pid, socketMemoria);
  pthread_mutex_unlock(&operandoConMemoria);
  terminarConsola(consolaFinalizada, posicionProceso);
}

bool estaEnTablaGlobal (char* nomArchivo) {
  for (int i = 0; i < list_size(tablaGlobalDeArchivos); i++) {
	  tablaGlobal* tablaActual = list_get(tablaGlobalDeArchivos, i);

    if (strcmp(nomArchivo, tablaActual->nomArchivo) == 0) {
      return true;  // Name found in the list
    }
  }
  return false;
}

void agregarATabla (char* nombreArchivo) {
    tablaGlobal* tabla1 = malloc(sizeof(tablaGlobal));
    tabla1->nomArchivo = nombreArchivo;
    tabla1->colaBloqueado =  queue_create();
    list_add(tablaGlobalDeArchivos, tabla1);
}

tablaGlobal* buscarEnTablaGlobal(char* nombreArchivo) {
  tablaGlobal* tabla;
   for (int i = 0; i < list_size(tablaGlobalDeArchivos); i++) {
	  tablaGlobal* tablaActual = list_get(tablaGlobalDeArchivos, i);
        if (strcmp(nombreArchivo, tablaActual->nomArchivo) == 0) {
            tabla = tablaActual;
        }
    }
    return tabla;  
}

void bloquearEnCola(char* nombreArchivo, PCB* proceso) {
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombreArchivo);
  queue_push(tablaEncontrada -> colaBloqueado, proceso);
}

int encontrarEnTablaDeArchivos(t_list* tablaArchivos, char* nombre ) {
  int posicion;  
    for (int i = 0; i < list_size(tablaArchivos); i++){
      archivoAbierto* arch = list_get(tablaArchivos,i);
      if(strcmp(nombre,arch->nombre)==0){
            posicion = i;
            break;
      }
    }
  return posicion;
}

int encontrarEnTablaGlobal(char* nombre) {
  int posicion;  
    for (int i = 0; i < list_size(tablaGlobalDeArchivos); i++){
      tablaGlobal* arch = list_get(tablaGlobalDeArchivos,i);
      if(strcmp(nombre,arch->nomArchivo)==0){
            posicion = i;
            break;
      }
    }
    return posicion;
}

int obtenerPosicion(char* nomArchivo, contextoEjecucion* contexto){
  int posicion = encontrarEnTablaDeArchivos(contexto-> archivosAbiertos,nomArchivo);
  archivoAbierto* arch = list_get(contexto-> archivosAbiertos,posicion);
  int puntero = arch->punteroArchivo;
  return puntero;
}

void eliminarDeTablaDeArchivos(char* nombreArchivo,PCB* procesoDevuelto) {
  int posicion = encontrarEnTablaDeArchivos(procesoDevuelto -> contexto -> archivosAbiertos, nombreArchivo);
  //ver de hacer fclose aca
  list_remove(procesoDevuelto -> contexto -> archivosAbiertos, posicion);
}

bool hayEnCola(char* nombre) {
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombre);
  if(queue_size (tablaEncontrada -> colaBloqueado) >= 1 ){
    return true;
  }
  return false;
}

void moverAListoColaDeArchivo(char* nombreArchivo) {
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombreArchivo);
  PCB* procesoAListo = queue_pop (tablaEncontrada -> colaBloqueado);
  agregarAListo(procesoAListo);
}

void eliminarDeTablaGlobal(char* nombreArchivo) {
  int posicion = encontrarEnTablaGlobal(nombreArchivo);
  list_remove(tablaGlobalDeArchivos,posicion);
}

void iniciarTablaGlobal() {
  tablaGlobalDeArchivos = list_create();
}

void actualizarSegmentos(t_list* segmentosDesactualizados, t_list* segmentosNuevos) {
  int cantidadSegmentosDesactualizados = segmentosDesactualizados->elements_count - 1; // -1 por segmento 0
  int cantidadSegmentosNuevos = segmentosNuevos->elements_count;

  for (int i = 0; i < cantidadSegmentosDesactualizados; i++) {
    Segmento* segmentoViejo = list_get(segmentosDesactualizados, i + 1);
    Segmento* segmentoNuevo = list_get(segmentosNuevos, i);
    segmentoViejo->base = segmentoNuevo->base;
  }

  if (cantidadSegmentosDesactualizados != cantidadSegmentosNuevos) {
    Segmento* ultimoSegmento = list_get(segmentosNuevos, cantidadSegmentosNuevos - 1);
    Segmento* ultimoSegmentoNuevo = malloc(sizeof(Segmento));
    ultimoSegmentoNuevo->id = ultimoSegmento->id;
    ultimoSegmentoNuevo->base = ultimoSegmento->base;
    ultimoSegmentoNuevo->limite = ultimoSegmento->limite;
    list_add(segmentosDesactualizados, ultimoSegmentoNuevo);
  }
}

int actualizarSiEstaEjecutandose(int idProceso, t_list* segmentosProceso) {
  int procesoEncontrado = 0;
  if (procesoEjecutandose->pid == idProceso) {
    actualizarSegmentos(procesoEjecutandose->contexto->tablaSegmentos, segmentosProceso);
    procesoEncontrado = 1;
  }
  return procesoEncontrado;
}

int actualizarSiEstaEn(int idProceso, t_list* segmentosProceso, t_list* colaProcesos, pthread_mutex_t mutextCola) {
  int procesoEncontrado = 0;
  int cantidadDeProcesosEnReady = colaProcesos->elements_count;

  for (int i = 0; i < cantidadDeProcesosEnReady; i++) {
    pthread_mutex_lock(&mutextCola);
    PCB* pcb = list_get(colaProcesos, i);
    pthread_mutex_unlock(&mutextCola);
    if (pcb->pid == idProceso) {
      actualizarSegmentos(pcb->contexto->tablaSegmentos, segmentosProceso);
      procesoEncontrado = 1;
    }
  }
  return procesoEncontrado;
}

int actualizarSiEstaBloqueadoPorArchivo(int idProceso, t_list* segmentosProceso) {
  int cantidadColasDeArchivos = tablaGlobalDeArchivos->elements_count;
  int procesoEncontrado = 0;
  for (int i = 0; i < cantidadColasDeArchivos; i++) {
    tablaGlobal* tabla = list_get(tablaGlobalDeArchivos, i);
    t_list* listaProcesos = tabla->colaBloqueado->elements;
    procesoEncontrado = actualizarSiEstaEn(idProceso, segmentosProceso, listaProcesos, mutexColaArchivos);
  }
  return procesoEncontrado;
}

int actualizarSiEstaBloqueadoPorRecurso(int idProceso, t_list* segmentosProceso) {
  int cantidadDeRecursos = listaRecursos->elements_count;
  int procesoEncontrado = 0;
    for (int i = 0; i < cantidadDeRecursos; i++) {
      recursoSolicitados* recurso = list_get(listaRecursos, i);
      t_list* listaProcesos = recurso->colaBloqueados->elements;
      procesoEncontrado = actualizarSiEstaEn(idProceso, segmentosProceso, listaProcesos, mutexColaArchivos);
    }
    return procesoEncontrado;
}

void actualizarProceso(int idProceso, t_list* segmentosProceso) {
  if (actualizarSiEstaEjecutandose(idProceso, segmentosProceso)) {
  } else if (actualizarSiEstaEn(idProceso, segmentosProceso, colaReady, mutexColaReady)) {
  } else if (actualizarSiEstaEn(idProceso, segmentosProceso, colaBlock->elements, mutexColaBlock)) {
  } else if (actualizarSiEstaBloqueadoPorArchivo(idProceso, segmentosProceso)) {
  } else if (actualizarSiEstaBloqueadoPorRecurso(idProceso, segmentosProceso)) {
  }
}

void actualizarSegmentosProcesos(t_list* tablaDeSegmentos) {
  int cantidadProcesos = tablaDeSegmentos->elements_count;

  for (int i = 0; i < cantidadProcesos; i++) {
    tablaDeSegmento* tablaProceso = list_get(tablaDeSegmentos, i);
    actualizarProceso(tablaProceso->id, tablaProceso->segmentos_proceso);
  }
}

void agregarATablaArchivo(contextoEjecucion* contexto, char* nombreArchivo) {
  archivoAbierto* nuevoArchivo = malloc(sizeof(archivoAbierto));
  nuevoArchivo->nombre = nombreArchivo;
  nuevoArchivo->punteroArchivo = 0;
  list_add(contexto->archivosAbiertos, nuevoArchivo);
}

void ejecutar(PCB* proceso) {
  // Esta parte envia a cpu
  procesoEjecutandose = proceso;
  int socketCpu = recursosKernel->conexiones->socketCpu;
  log_info(recursosKernel->logger, "Envio proceso con PID: [%d] a CPU.", proceso->pid);
  cambiarEstado(EXEC, procesoEjecutandose);
  enviarContexto(proceso->contexto, socketCpu, Pcb);
  enviarEntero(proceso->pid, socketCpu);
  recibirInstruccion();
}

void recibirInstruccion() {
  PCB* proceso = procesoEjecutandose;
  int socketCpu = recursosKernel->conexiones->socketCpu;
  op_code codigoOperacion = obtenerCodigoOperacion(socketCpu);
  contextoEjecucion* nuevoContexto = recibirContexto(socketCpu);
  int socketMemoria = recursosKernel->conexiones->socketMemoria;
  int socketFileSystem = recursosKernel->conexiones->socketFileSystem;
  actualizarContexto(proceso, nuevoContexto);
  PCB* procesoDevuelto = procesoEjecutandose;

  switch (codigoOperacion) {
    case YIELD:
      sacarDeEjecutando(READY,procesoDevuelto);
      agregarAListo(procesoDevuelto);
      break;

    case F_TRUNCATE:
      char* nombreArchivoATruncar = recibirString(socketCpu);
      int tamanioNuevo = recibirEntero(socketCpu);
      log_info(
                  recursosKernel->logger,
                  "PID: <%d> Truncar Archivo -Nombre Archivo: <%s> - Tamaño: <%d>",
                  procesoDevuelto->pid, nombreArchivoATruncar,tamanioNuevo
                );
      PCB* procesoTruncado = procesoEjecutandose;
      sacarDeEjecutando(BLOCK,procesoDevuelto);
      agregar_proceso_bloqueado(procesoDevuelto);

      enviarContexto(procesoDevuelto->contexto,socketFileSystem, F_TRUNCATE);
      enviarString(nombreArchivoATruncar,socketFileSystem);
      enviarEntero(tamanioNuevo,socketFileSystem);

      op_code respuestaTruncado = obtenerCodigoOperacion(socketFileSystem);
      contextoEjecucion* nuevoTruncado = recibirContexto(socketFileSystem);

      switch(respuestaTruncado) {
        case SUCCESS_TRUNCATE:

        actualizarContexto(procesoTruncado, nuevoTruncado);

        agregarAListo(procesoDevuelto);

        break;

        default:
          puts("Entre por el default");
          break;
      }
      break;

    case F_OPEN:
      char* nombreArchivo = recibirString(socketCpu);
      log_info(
                 recursosKernel->logger,
                 "PID: <%d> - Abrir Archivo - Nombre Archivo: <%s>",
                 procesoDevuelto->pid, nombreArchivo
               );
        if(estaEnTablaGlobal(nombreArchivo)) {
          agregarATablaArchivo(procesoDevuelto->contexto, nombreArchivo);
          enviarContexto(procesoDevuelto->contexto, socketCpu, BLOCK);
          log_info(
                   recursosKernel->logger,
                   "PID: <%d> - Bloqueado por: <%s>",
                   procesoDevuelto->pid, nombreArchivo
                 );
          sacarDeEjecutando(BLOCK,procesoDevuelto);
          bloquearEnCola(nombreArchivo, procesoDevuelto);
        } else {
          agregarATabla(nombreArchivo);
          enviarContexto(procesoDevuelto->contexto, socketFileSystem, F_OPEN);
          enviarString(nombreArchivo, socketFileSystem);

          op_code respuestaFS = obtenerCodigoOperacion(socketFileSystem);
          contextoEjecucion* nuevoFS = recibirContexto(socketFileSystem);

          switch(respuestaFS) {
            case SUCCESS_OPEN:
              actualizarContexto(procesoEjecutandose, nuevoFS);
              agregarATablaArchivo(nuevoFS, nombreArchivo);
              enviarContexto(nuevoFS, socketCpu, SUCCESS_OPEN_CPU);
              recibirInstruccion();
              break;
            default:
              puts("Entre por el default de F_OPEN");
              break;
          }
          //recibir contexto y enviarlo a CPU
        }
      break;

    case DELETE_SEGMENT:
      int idSeg = recibirEntero(socketCpu);
      log_info(
                  recursosKernel->logger,
                  "PID: <%d> - Eliminar Segmento - ID segmento: <%d>",
                  procesoDevuelto->pid, idSeg
                );
      pthread_mutex_lock(&operandoConMemoria);
      enviarContexto(procesoDevuelto->contexto, socketMemoria, DELETE_SEGMENT);
      enviarEntero(procesoEjecutandose->pid, socketMemoria);
      enviarEntero(idSeg, socketMemoria);

      obtenerCodigoOperacion(socketMemoria);
      nuevoContexto = recibirContexto(socketMemoria);
      pthread_mutex_unlock(&operandoConMemoria);
      actualizarContexto(procesoEjecutandose, nuevoContexto);
      enviarEntero(7, socketCpu);
      enviarContexto(procesoEjecutandose->contexto, socketCpu, SUCCESS);
      recibirInstruccion();
      break;
    case F_CLOSE:
      char* nombrArchivo = recibirString(socketCpu);
      log_info(
                 recursosKernel->logger,
                 "PID: <%d> - Cerrar Archivo - Nombre Archivo: <%s>",
                 procesoDevuelto->pid, nombrArchivo
               );
      eliminarDeTablaDeArchivos(nombrArchivo, procesoDevuelto);
      if(hayEnCola(nombrArchivo)){
        moverAListoColaDeArchivo(nombrArchivo);
      } else {
        eliminarDeTablaGlobal(nombrArchivo);
      }
      enviarContexto(procesoDevuelto->contexto,socketCpu,SUCCESS_CLOSE);
      free(nombrArchivo);
      recibirInstruccion();
      break;

    case F_SEEK:

      char* nomArchivo = recibirString(socketCpu);
      int posicion = recibirEntero(socketCpu);
      log_info(
                  recursosKernel->logger,
                  "PID: <%d> -Actualizar Puntero Archivo - Nombre Archivo: <%s> - Puntero: <%d>",
                  procesoDevuelto->pid, nomArchivo,posicion
                );

      t_list* archivosAbiertos = procesoDevuelto->contexto->archivosAbiertos;
      int posicionEnTabla = encontrarEnTablaDeArchivos(archivosAbiertos,nomArchivo);
     archivoAbierto* arch = list_get(archivosAbiertos, posicionEnTabla);
     arch->punteroArchivo = posicion;
      enviarContexto(procesoDevuelto->contexto,socketCpu,SUCCESS);
      free(nomArchivo);
      recibirInstruccion();
      break;

    case F_READ:

      PCB* procesoQueLeyo = procesoEjecutandose;
      sacarDeEjecutando(BLOCK,procesoDevuelto);
      agregar_proceso_bloqueado(procesoDevuelto);


      char* nombreArchivoALeer = recibirString(socketCpu);
      int posicionEnMemoriaAleer = recibirEntero(socketCpu);
      int tamanioALeer = recibirEntero(socketCpu);
      int posicionDeArchivoLectura = obtenerPosicion(nombreArchivoALeer,procesoDevuelto->contexto);
      log_info(
                  recursosKernel->logger,
                  "PID: <%d> Leer Archivo -Nombre Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamaño : <%d>",
                  procesoDevuelto->pid, nombreArchivoALeer,posicionDeArchivoLectura,posicionEnMemoriaAleer,tamanioALeer
                );
      usoDeMemoria ++;
      sem_wait(&usoMemoria);
      enviarContexto(procesoDevuelto->contexto,socketFileSystem,F_READ);
      enviarString(nombreArchivoALeer,socketFileSystem);
      enviarEntero(tamanioALeer,socketFileSystem);
      enviarEntero(posicionEnMemoriaAleer,socketFileSystem);
      enviarEntero(posicionDeArchivoLectura,socketFileSystem);
      op_code respuestaLectura = obtenerCodigoOperacion(socketFileSystem);

      contextoEjecucion* nuevoLeido = recibirContexto(socketFileSystem);
      sem_post(&usoMemoria);
      usoDeMemoria --;
      switch(respuestaLectura) {
        case SUCCESS_READ:
        actualizarContexto(procesoQueLeyo, nuevoLeido);
        agregarAListo(procesoDevuelto);
        break;
        default:
          puts("Entre por default de SUCCESS_write");
          break;
      }
      break;

    case F_WRITE:
      PCB* procesoQueEscribio = procesoEjecutandose;

      sacarDeEjecutando(BLOCK,procesoDevuelto);
      agregar_proceso_bloqueado(procesoDevuelto);
      char* nombreArchivoAEscribir = recibirString(socketCpu);
      int posicionEnMemoria = recibirEntero(socketCpu);
      int tamanioAEscribir = recibirEntero (socketCpu);
      int posicionDeArchivo = obtenerPosicion(nombreArchivoAEscribir,procesoDevuelto->contexto);
      log_info(
                 recursosKernel->logger,
                 "PID: <%d> Escribir Archivo -Nombre Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamaño : <%d>",
                 procesoDevuelto->pid, nombreArchivoAEscribir,posicionDeArchivo,posicionEnMemoria,tamanioAEscribir
               );
      usoDeMemoria ++;
      sem_wait(&usoMemoria);
      enviarContexto(procesoDevuelto->contexto,socketFileSystem,F_WRITE);
      enviarString(nombreArchivoAEscribir,socketFileSystem);
      enviarEntero(posicionEnMemoria,socketFileSystem);
      enviarEntero(tamanioAEscribir,socketFileSystem);
      enviarEntero(posicionDeArchivo,socketFileSystem);
      op_code respuestaEscritura = obtenerCodigoOperacion(socketFileSystem);
      contextoEjecucion* nuevoEscrito = recibirContexto(socketFileSystem);
      sem_post(&usoMemoria);
      usoDeMemoria --;


      switch(respuestaEscritura) {
        case SUCCESS_WRITE:
        actualizarContexto(procesoQueEscribio, nuevoEscrito);
        agregarAListo(procesoDevuelto);
        break;

        default:
          puts("Entre por el default");
          break;
      }
      break;

    case IO:

      procesoDevuelto->tiempoBloqueadoIO = recibirEntero(socketCpu);
      log_info(
            recursosKernel->logger,
            "PID: <%d> - Bloqueado por: <IO>",
            procesoDevuelto->pid
          );
      sacarDeEjecutando(BLOCK,procesoDevuelto);
      agregar_proceso_bloqueado_io(procesoDevuelto);
      break;

    case EXIT:

      PCB* procesoTerminado = procesoDevuelto;
      sacarDeEjecutando(EXITSTATE,procesoDevuelto);
      log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: SUCCESS", proceso->pid);
      finalizarProceso(procesoTerminado, SUCCESS);
      liberarPcb(procesoTerminado);
      break;

    case WAIT:
      char* recursoPedido = recibirString(socketCpu);
      int posicionRecurso = validarRecurso(recursoPedido);
      int cantidadDeInstancias = darInstanciasRecurso(posicionRecurso);
      log_info(
                recursosKernel->logger,
                "PID: <%d> - Wait: <%s>- Cantidad de Instancias: <%d>",
                procesoDevuelto->pid, recursoPedido,cantidadDeInstancias-1
              );
      if (posicionRecurso >= 0) {
        if(validarInstanciasDeRecurso(posicionRecurso)) {
          disminuirInstanciasRecurso(posicionRecurso);

          enviarContexto(procesoDevuelto->contexto, socketCpu, SUCCESS);
          recibirInstruccion();
        } else {
          disminuirInstanciasRecurso(posicionRecurso);
          log_info(recursosKernel->logger, "Proceso: [%d] se movio a BLOQUEADO", procesoDevuelto->pid);
                    log_info(
                      recursosKernel->logger,
                      "PID: <%d> - Bloqueado por: <%s>",
                      procesoDevuelto->pid, recursoPedido
                    );
          bloquearProcesoPorRecurso(procesoDevuelto, posicionRecurso);
          enviarContexto(procesoDevuelto->contexto, socketCpu, BLOCK);
          sacarDeEjecutando(BLOCK,procesoDevuelto);
        }
      } else {
        PCB* procesoTerminado = procesoEjecutandose;
        enviarContexto(procesoDevuelto->contexto, socketCpu, BLOCK);
        sacarDeEjecutando(EXIT,procesoDevuelto);
        log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: INVALID RESOURCE", proceso->pid);
        finalizarProceso(procesoDevuelto, INVALID_RESOURCE);
        liberarPcb(procesoTerminado);
      }
      free(recursoPedido);
      break;

    case SIGNAL:

      char* recursoPedidoSignal = recibirString(socketCpu);
      int posicionRecursoSignal = validarRecurso(recursoPedidoSignal);
      int cantidad = darInstanciasRecurso(posicionRecursoSignal);
             log_info(
                recursosKernel->logger,
                "PID: <%d> - Signal: <%s>- Cantidad de Instancias: <%d>",
                procesoDevuelto->pid, recursoPedidoSignal,cantidad+1
              );
      if (posicionRecursoSignal >= 0) {
        aumentarRecurso(posicionRecursoSignal);
        if(validarInstanciasDeRecurso(posicionRecursoSignal)) {
          enviarContexto(procesoDevuelto->contexto, socketCpu, SUCCESS);

          recibirInstruccion();
        } else {

          PCB* procesoBloqueado = obtenerProcesoBloqueado(posicionRecursoSignal);
          log_info(recursosKernel->logger, "Proceso: [%d] se movio a LISTO", procesoBloqueado->pid);
          enviarContexto(procesoDevuelto->contexto, socketCpu, SUCCESS);
          agregarAListo(procesoBloqueado);
          recibirInstruccion();
        }
      } else {
        PCB* procesoTerminado = procesoEjecutandose;
        enviarContexto(procesoDevuelto->contexto, socketCpu, EXIT);
        sacarDeEjecutando(EXIT,procesoDevuelto);
        log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: INVALID RESOURCE", proceso->pid);
        finalizarProceso(procesoDevuelto, INVALID_RESOURCE);
        liberarPcb(procesoTerminado);
      }
      free(recursoPedidoSignal);
      break;

    case CREATE_SEGMENT:
      int idSegmento = recibirEntero(socketCpu);
      int tamanioSegmento = recibirEntero(socketCpu);
      pthread_mutex_lock(&operandoConMemoria);
      log_info(
                  recursosKernel->logger,
                  "PID: <%d> - Crear Segmento - ID segmento: <%d> - Tamaño: <%d> ",
                  procesoDevuelto->pid, idSegmento,tamanioSegmento
                );
      enviarContexto(procesoDevuelto->contexto, socketMemoria, CREATE_SEGMENT);
      enviarEntero(procesoEjecutandose->pid, socketMemoria);
      enviarEntero(idSegmento, socketMemoria);
      enviarEntero(tamanioSegmento, socketMemoria);
      op_code respuestaMemoria = obtenerCodigoOperacion(socketMemoria);

      switch(respuestaMemoria) {
        case Pcb:
          contextoEjecucion* nuevoActualizado = recibirContexto(socketMemoria);
          actualizarContexto(procesoEjecutandose, nuevoActualizado);
          enviarContexto(procesoEjecutandose->contexto, socketCpu, SUCCESS);
          pthread_mutex_unlock(&operandoConMemoria);
          recibirInstruccion();
          break;

        case COMPACTACION:
          contextoEjecucion* contextoAlRePedo = recibirContexto(socketMemoria);
          log_info(recursosKernel->logger, "Se solicitó compactación");
          if(usoDeMemoria > 0){
        	  log_info(recursosKernel->logger, "Esperando a que termine File System");
                    }
          sem_wait(&usoMemoria);
          obtenerCodigoOperacion(socketMemoria);
          t_list* tablaDeSegmentos = recibirTablaDeSegmentos(socketMemoria);
          sem_post(&usoMemoria);
          log_info(recursosKernel->logger, "Se finalizó el proceso de compactación");
          actualizarSegmentosProcesos(tablaDeSegmentos);
          enviarContexto(procesoEjecutandose->contexto, socketCpu, SUCCESS);
          pthread_mutex_unlock(&operandoConMemoria);
          recibirInstruccion();
          break;

        case OUT_OF_MEMORY:
          contextoEjecucion* recibeAlgoAlPedo = recibirContexto(socketMemoria);
          sacarDeEjecutando(EXIT,procesoDevuelto);
          log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: OUT OF MEMORY", proceso->pid);
          pthread_mutex_unlock(&operandoConMemoria);
          enviarContexto(procesoDevuelto->contexto, socketCpu, OUT_OF_MEMORY);
          finalizarProceso(procesoDevuelto, OUT_OF_MEMORY);
          break;
        default:
          puts("Como carajo llegue al defaul de create segment");
          break;
      }

      break;
      case SEGMENTATION_FAULT:
          sacarDeEjecutando(EXIT,procesoDevuelto);
          log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: SEGMENTATION FAULT", proceso->pid);
          finalizarProceso(procesoDevuelto, SEGMENTATION_FAULT);
          break;

    default:
      puts("-------------------- Entre por default --------------------");
      break;
  }
}

void agregarFinalizado(PCB* proceso) {
    pthread_mutex_lock(&mutexColaEnd);
    t_log* logger = recursosKernel->logger;
    queue_push(colaEnd, proceso);
    log_info(logger, "Proceso: [%d] se encuentra FINALIZADO.", proceso->pid);

    pthread_mutex_unlock(&mutexColaEnd);

    sem_post(&largoPlazo);
}

int findElementPosition(char array[], int size, char* target) {
    for (int i = 0; i < size; i++) {
        if (array[i] == *target) {
            return i;
        }
    }
    return -1;  // Return -1 if the element is not found
}


/*

bool kernelTieneRecurso(char* recurso){
  int tamanioArray = sizeof(recursosKernel->configuracion->RECURSOS);
  int position = findElementPosition(recursosKernel->configuracion->RECURSOS,tamanioArray, recurso);
  int position = findElementPosition(recursosKernel->configuracion->RECURSOS) ;
  return position != -1;
}

bool hayRecursoDisponible(char* recurso) {
  int tamanioArray = sizeof(recursosKernel->configuracion->RECURSOS);
  int position = findElementPosition(recursosKernel->configuracion->RECURSOS,tamanioArray, recurso);
  int cantidad = recursosKernel->configuracion->INSTANCIAS_RECURSOS[position] ;
  return cantidad > 0;
}

void aumentarRecurso(char* recurso){
  int tamanioArray = sizeof(recursosKernel->configuracion->RECURSOS);
  int position = findElementPosition(recursosKernel->configuracion->RECURSOS,tamanioArray, recurso);
  ++recursosKernel->configuracion->INSTANCIAS_RECURSOS[position];
}
void disminuirRecurso(char* recurso){
  int tamanioArray = sizeof(recursosKernel->configuracion->RECURSOS);
  int position = findElementPosition(recursosKernel->configuracion->RECURSOS,tamanioArray, recurso);
  --recursosKernel->configuracion->INSTANCIAS_RECURSOS[position];
}
*/


void liberar_semaforos() {
    pthread_mutex_destroy(&mutexNumeroProceso);
    pthread_mutex_destroy(&mutexProcesoListo);
    pthread_mutex_destroy(&mutexColaNew);
    pthread_mutex_destroy(&mutexColaReady);
    pthread_mutex_destroy(&mutexColaBlock);
    pthread_mutex_destroy(&mutexColaExec);
    pthread_mutex_destroy(&mutexColaEnd);
    pthread_mutex_destroy(&mutex_cola);
    pthread_mutex_destroy(&mutexcantidadProcesosMemoria);

    sem_destroy(&semProcesoNew);
    sem_destroy(&semProcesoReady);
    sem_destroy(&semaProcesoExec);
    sem_destroy(&blockCounter);
    sem_destroy(&largoPlazo);
    sem_destroy(&semaforoCantidadProcesosExec);
}

void liberar_estructuras() {

    list_destroy(colaReady);

    queue_destroy(colaNew);

    queue_destroy(colaBlock);

    //queue_destroy(colaExec);

    queue_destroy(colaEnd);
}

