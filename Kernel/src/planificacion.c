#include <planificacion.h>
#include <estructuras.h>
#include <recursos.h>
#include <serializacion/contexto.h>
#include <serializacion/buffer.h>
#include <serializacion/paquete.h>
#include <conexiones.h>
#include <utils.h>

t_list* tablaGlobalDeArchivos;

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
  int rafagaPrevia = proceso->rafagaRealPrevia * 1000 ;
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
  list_sort(colaReady, &ordenarSegunCalculoHRRN);
  procesoAEjecutar = list_remove (colaReady,0);
  pthread_mutex_unlock(&mutexColaReady);
  return procesoAEjecutar;
}

void planificador_corto_plazo_HRRN() {
  t_log* logger = recursosKernel->logger;
  log_info(logger, "INICIO PLANIFICACION FIFO");
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
  log_info(logger, "----------[DISP I/O] Proceso: [%d] ,se bloqueara %f segundos.----------", proceso->pid, tiempoBloqueo / 1000.0);
  int tiempoBloqueoEnMicrosegundos = tiempoBloqueo * 1000;
  usleep(tiempoBloqueoEnMicrosegundos);
  log_info(logger, "----------[DISP I/O] Proceso: [%d] ,termino I/O %f segundos.----------", proceso->pid, tiempoBloqueo / 1000.0);

  proceso = sacarBloqueado();
  agregarAListo(proceso);
  }
}

void agregar_proceso_bloqueado(PCB *procesoBloqueado) {
    procesoBloqueado->estimadoRafaga = estimacion(procesoBloqueado);
    procesoBloqueado->rafagaRealPrevia = calcular_tiempo_rafaga_real_anterior(procesoBloqueado);
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
  return (config->GRADO_MAX_MULTIPROGRAMACION > list_size(colaReady) && queue_size(colaNew) > 0)? 1 : 0;
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

void agregarAListo(PCB* proceso) {
  t_log* logger = recursosKernel->logger;

  pthread_mutex_lock(&mutexColaReady);
  list_add(colaReady, proceso);
  log_info(logger, "Proceso: [%d] se movio a listo", proceso->pid);
  pthread_mutex_unlock(&mutexColaReady);
  sem_post(&semProcesoReady);
}

void sacarDeEjecutando(estadoProceso estado) {
  t_log* logger = recursosKernel->logger;

  pthread_mutex_lock(&mutexColaExec);
  log_info(logger, "Proceso: [%d] salío de EJECUTANDO.", procesoEjecutandose->pid);
  cambiarEstado(estado, procesoEjecutandose);
  procesoEjecutandose = NULL;
  pthread_mutex_unlock(&mutexColaExec);
  sem_post(&semaforoCantidadProcesosExec);
}

void actualizarContexto(contextoEjecucion* nuevoContexto){
  liberarContexto(procesoEjecutandose->contexto);
  procesoEjecutandose->contexto = nuevoContexto;
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

  avisarProcesoFinalizado(socketConsola, motivo);
  avisarProcesoFinalizado(socketMemoria, motivo);
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
    tabla1 -> nomArchivo = nombreArchivo;
    t_queue* colaArchivo = queue_create();
    tabla1 -> colaBloqueado = colaArchivo;
}
tablaGlobal* buscarEnTablaGlobal(char* nombreArchivo) {
  tablaGlobal* tabla;
   for (int i = 0; i < list_size(tablaGlobalDeArchivos); i++) {
	  tablaGlobal* tablaActual = list_get(tablaGlobalDeArchivos, i);
        if (strcmp(nombreArchivo, tablaActual->nomArchivo) == 0) {
            tabla = tablaActual;
            break;
        }
    }
    return tabla;  
}

void bloquearEnCola(char* nombreArchivo, PCB* proceso) {
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombreArchivo);
  queue_push (tablaEncontrada -> colaBloqueado,proceso);
}

int encontrarEnTablaDeArchivos(t_list* tablaArchivos, char* nombre ){
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

int encontrarEnTablaGlobal(char* nombre ){
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

void eliminarDeTablaDeArchivos(char* nombreArchivo,PCB* procesoDevuelto){
  int posicion = encontrarEnTablaDeArchivos(procesoDevuelto -> contexto -> archivosAbiertos, nombreArchivo);
  //ver de hacer fclose aca
  list_remove(procesoDevuelto -> contexto -> archivosAbiertos, posicion);
}

bool hayEnCola(char* nombre){
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombre);
  if(queue_size (tablaEncontrada -> colaBloqueado) >= 1 ){
    return true;
  }
  return false;
}

void moverAListoColaDeArchivo(char* nombreArchivo){
  tablaGlobal* tablaEncontrada = buscarEnTablaGlobal(nombreArchivo);
  PCB* procesoAListo = queue_pop (tablaEncontrada -> colaBloqueado);
  agregarAListo(procesoAListo);
}

void eliminarDeTablaGlobal(char* nombreArchivo){
  int posicion = encontrarEnTablaGlobal(nombreArchivo);
  list_remove(tablaGlobalDeArchivos,posicion);
}

void iniciarTablaGlobal() {
  tablaGlobalDeArchivos = list_create();
}

void ejecutar(PCB* proceso) {
  procesoEjecutandose = proceso;
  int socketCpu = recursosKernel->conexiones->socketCpu;
  int socketMemoria = recursosKernel->conexiones->socketMemoria;
  int socketFileSystem = recursosKernel->conexiones->socketFileSystem;
  log_info(recursosKernel->logger, "Envio proceso con PID: [%d] a CPU.", proceso->pid);
  cambiarEstado(EXEC, procesoEjecutandose);
  enviarContexto(proceso->contexto, socketCpu, Pcb);
  op_code codigoOperacion = obtenerCodigoOperacion(socketCpu);
  contextoEjecucion* nuevoContexto = recibirContexto(socketCpu);
  actualizarContexto(nuevoContexto);
  PCB* procesoDevuelto = procesoEjecutandose;

  switch (codigoOperacion) {
    case YIELD:
      puts("-------------------- Llego YIELD --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case MOV_IN:
      puts("-------------------- Llego MOV_IN --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case F_TRUNCATE:
      puts("-------------------- Llego F_TRUNCATE --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case MOV_OUT:
      puts("-------------------- Llego MOV_OUT --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case F_OPEN:
      puts("-------------------- Llego F_OPEN --------------------");
      char* nombreArchivo = recibirString(socketCpu);
      printf("Recibi archivo con nombre %s\n", nombreArchivo);
        if(estaEnTablaGlobal(nombreArchivo)){
          sacarDeEjecutando(BLOCK);
          bloquearEnCola(nombreArchivo, procesoDevuelto);
        } else {
          agregarATabla(nombreArchivo);
          // Calculo que se agrega a ready luego
          sacarDeEjecutando(READY);
          agregarAListo(procesoDevuelto);
          //enviarContexto(procesoDevuelto->contexto,socketFileSystem, F_OPEN);
          //enviarString(nombreArchivo, socketFileSystem);
          //esperarCodOperacion
          //recibir contexto y enviarlo a CPU
        }
        free(nombreArchivo);
      break;
    case DELETE_SEGMENT:
      puts("-------------------- Llego DELETE_SEGMENT --------------------");
      int idSeg = recibirEntero(socketCpu);

      enviarContexto(procesoDevuelto->contexto, socketMemoria, DELETE_SEGMENT);
      enviarEntero(procesoEjecutandose->pid, socketMemoria);
      enviarEntero(idSeg, socketMemoria);
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case F_CLOSE:
      puts("-------------------- Llego F_CLOSE --------------------");
      sacarDeEjecutando(READY);
      char* nombrArchivo = recibirString(socketCpu);
      printf("Nombre archivo %s\n", nombrArchivo);
      /*
      eliminarDeTablaDeArchivos(nombrArchivo, procesoDevuelto);
      if(hayEnCola(nombrArchivo)){
        moverAListoColaDeArchivo(nombrArchivo);
      } else {
        eliminarDeTablaGlobal(nombrArchivo);
      }
      */
      agregarAListo(procesoDevuelto);
      free(nombrArchivo);
      break;
    case F_SEEK:
      puts("-------------------- Llego F_SEEK --------------------");
      char* nomArchivo = recibirString(socketCpu);
      int posicion = recibirEntero(socketCpu);
      t_list* archivosAbiertos = procesoDevuelto->contexto->archivosAbiertos;
      // Abria que tener el archivo para poder hacer fseek del mismo
      //procesoDevuelto->contexto->archivosAbiertos = f_seek(nomArchivo, archivosAbiertos, posicion);
      //enviarContexto(procesoDevuelto->contexto, socketCpu, SUCCESS);
      // Por ahora dejo que entre a ready
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      free(nomArchivo);
      break;
    case F_READ:
      puts("-------------------- Llego F_READ --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case F_WRITE:
      puts("-------------------- Llego F_WRITE --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
    case IO:
      puts("-------------------- Llego IO --------------------");
      procesoDevuelto->tiempoBloqueadoIO = recibirEntero(socketCpu);
      sacarDeEjecutando(BLOCK);
      agregar_proceso_bloqueado(procesoDevuelto);
      break;
    case EXIT:
      puts("-------------------- Llego Exit --------------------");
      PCB* procesoTerminado = procesoDevuelto;
      sacarDeEjecutando(EXITSTATE);
      log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: SUCCESS", proceso->pid);
      finalizarProceso(procesoTerminado, SUCCESS);
      //liberarPcb(procesoTerminado);
      break;
    case WAIT:
      puts("-------------------- Llego WAIT --------------------");
      char* recursoWait = recibirString(socketCpu);
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      free(recursoWait);
      break;
    case SIGNAL:
      puts("-------------------- Llego SIGNAL --------------------");
      char* recursoSignal = recibirString(socketCpu);
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      free(recursoSignal);
      break;
    case CREATE_SEGMENT:
      puts("-------------------- Llego CREATE_SEGMENT --------------------");
      printf("Entre en create_segment, codigo Operacion %d\n", codigoOperacion);
      int idSegmento = recibirEntero(socketCpu);
      int tamanioSegmento = recibirEntero(socketCpu);

      enviarContexto(procesoDevuelto->contexto, socketMemoria, CREATE_SEGMENT);
      enviarEntero(procesoEjecutandose->pid, socketMemoria);
      enviarEntero(idSegmento, socketMemoria);
      enviarEntero(tamanioSegmento, socketMemoria);
      op_code respuestaMemoria = obtenerCodigoOperacion(socketMemoria);
      contextoEjecucion* nuevoActualizado = recibirContexto(socketMemoria);

      printf("Respuesta memoria %d\n", respuestaMemoria);
      switch(respuestaMemoria) {
        case Pcb:
          actualizarContexto(nuevoActualizado);
          sacarDeEjecutando(READY);
          agregarAListo(procesoDevuelto);
          break;
        case OUT_OF_MEMORY:
          sacarDeEjecutando(EXIT);
          log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: OUT OF MEMORY", proceso->pid);
          finalizarProceso(procesoDevuelto, OUT_OF_MEMORY);
          break;
        default:
          puts("Como carajo llegue al defaul de create segment");
          break;
      }
      break;
      case SEGMENTATION_FAULT:
          sacarDeEjecutando(EXIT);
          log_info(recursosKernel->logger, "Finaliza el Proceso [%d], Motivo: SEGMENTATION FAULT", proceso->pid);
          finalizarProceso(procesoDevuelto, SEGMENTATION_FAULT);
          break;
    default:
      puts("-------------------- Entre por default --------------------");
      sacarDeEjecutando(READY);
      agregarAListo(procesoDevuelto);
      break;
  }
       //sacarDeEjecutando(procesoRecibido);
       //agregarFinalizado(procesoRecibido)
       //pid = procesoRecibido->pid;
       //paquete = crear_paquete(FINALIZAR);
       //agregar_a_paquete(paquete, &pid, sizeof(unsigned int));
       //semwait(&comunicacionMemoria);
       //enviar_paquete_a_servidor(paquete, socketMemoria);
       //log_info(logger, "Se envio el proceso %d a la memoria para finalizar", pid);
       //confirmar que llego a memoria
       //sem_post(&comunicacionMemoria);

  /*
    Pcb *procesoRecibido;
    procesoRecibido = deserializar_pcb(socketCPU);
    switch (codOperacion) {
    case YIELD:
        sacarDeEjecutando(procesoRecibido);
        agregarAListo(procesoRecibido)
        break;
    case SIGNAL:
        char* recurso = obtenerRecursoDePaquete(socketCPU);
        if(kernelTieneRecurso(recurso)){
          aumentarRecurso(recurso);
          enviarMensaje("Signal ejecutado correctamente", socket CPU)
        }
        else{
          enviarMensaje("Error, recurso no encontrado", socket CPU)
        }
        break;
    case WAIT:

        char* recurso = obtenerRecursoDePaquete(socketCPU);
        if(kernelTieneRecurso(recurso) && hayRecursoDisponible(recurso)){
          disminuirRecurso(recurso);
          enviarMensaje("Wait ejecutado correctamente", socket CPU)
        }
        else{
          enviarMensaje("Error", socket CPU)
        }
        break;

    case EXIT:
    sacarDeEjecutando(procesoRecibido);
        agregarFinalizado(procesoRecibido)
        pid = procesoRecibido->pid;
        paquete = crear_paquete(FINALIZAR);
        agregar_a_paquete(paquete, &pid, sizeof(unsigned int));
        semwait(&comunicacionMemoria);
        enviar_paquete_a_servidor(paquete, socketMemoria);
        log_info(logger, "Se envio el proceso %d a la memoria para finalizar", pid);
        //confirmar que llego a memoria
        sem_post(&comunicacionMemoria);
        break;
    case DESCONEXION:
        log_info(logger, "Se desconectó el CPU-Dispatch. %d", codOperacion);
        break;

    default:
        log_warning(logger, "Operación desconocida.");
        break;
    }
  */
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

