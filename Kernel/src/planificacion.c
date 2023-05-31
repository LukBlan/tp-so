#include <planificacion.h>
#include <estructuras.h>
#include <recursos.h>

t_queue* colaNew;
t_list* colaReady;
t_queue* colaExec;
t_queue* colaBlock;
t_queue* colaEnd;

pthread_mutex_t mutexNumeroProceso;
pthread_mutex_t mutexProcesoListo;

pthread_mutex_t mutexColaNew;
pthread_mutex_t mutexColaReady;
pthread_mutex_t mutexColaBlock;
pthread_mutex_t mutexColaExec;
pthread_mutex_t mutexColaEnd;

pthread_mutex_t mutex_cola;
pthread_mutex_t mutexcantidadProcesosMemoria;

sem_t semProcesoNew;
sem_t semProcesoReady;
sem_t semaProcesoExec;

sem_t blockCounter;

sem_t largoPlazo;

sem_t semaforoCantidadProcesosExec;
sem_t comunicacionMemoria;


void iniciarColas() {
  colaNew = queue_create();
  colaReady = list_create();
  colaExec = queue_create();
  colaBlock = queue_create();
  colaEnd = queue_create();
}

void iniciarSemaforos() {
  pthread_mutex_init(&mutexNumeroProceso, NULL);

  pthread_mutex_init(&mutexColaNew, NULL);
  pthread_mutex_init(&mutexColaReady, NULL);
  pthread_mutex_init(&mutexColaExec, NULL);
  pthread_mutex_init(&mutexColaBlock, NULL);
  pthread_mutex_init(&mutexColaEnd, NULL);

  sem_init(&semProcesoNew, 0, 0);
  sem_init(&semProcesoReady, 0, 0);

  sem_init(&blockCounter, 0, 0);

  sem_init(&semaforoCantidadProcesosExec, 0, 1);

  sem_init(&largoPlazo, 0, 0);

}

void comenzarPlanificadores() {
  pthread_t hilo_largo_plazo;
  pthread_t hiloCortoPlazo;

  pthread_create(&hilo_largo_plazo, NULL, (void*) planificador_largo_plazo, NULL);
  pthread_detach(hilo_largo_plazo);

  if (strcmp(recursosKernel->configuracion->ALGORITMO_PLANIFICACION, "FIFO") == 0) {
    //pthread_create(&hiloCortoPlazo, NULL, (void*)planificador_corto_plazo_fifo, NULL);
    //pthread_detach(hiloCortoPlazo);
  }
  /*
  else
  {
    pthread_create(&hiloCortoPlazo, NULL, planificador_corto_plazo_HRRN, NULL);
    pthread_detach(hiloCortoPlazo);
  }

  pthread_create(&hilo_dispositivo_io, NULL, dispositivo_io, NULL);
  pthread_detach(hilo_dispositivo_io);
  */
}

void planificador_largo_plazo() {
  //log_info(loggerPlanificacion, "Inicio planificacion LARGO PLAZO en [%s]", KERNEL_CONFIG.ALGORITMO_PLANIFICACION);
  while (1) {
    sem_wait(&largoPlazo);
    // log_info(loggerPlanificacion, "[LARGO-PLAZO] Procesos en MEMORIA: %d", cantidadProcesosEnMemoria);
/*
    if (sePuedeAgregarMasProcesos()) {
      PCB *procesoSaliente = queue_pop(colaNew);
      cambiarEstado(READY, procesoSaliente);
      agregarAListo(procesoSaliente);
    }
   */
  }

}

void ejecutar(PCB* proceso) {
  //serializar la pcb
  //enviarla a cpu
  // se queda esperando el codOperacion
  // con ese codigo hace algo

  /*
    enviar_pcb(proceso, socketCPU);
    //log_info(logger, "Envio proceso con PID: [%d] por CPU-Dispatch.", proceso->pid);

    op_code codOperacion = obtener_codigo_operacion(socketCPU);

    Pcb *procesoRecibido;
    procesoRecibido = deserializar_pcb(socketCPU);
    switch (codOperacion) {
    case BLOQUEADOIO:
        agregar_proceso_bloqueado(procesoRecibido);
    case EXIT:
        agregarFinalizado()
        pid = procesoRecibido->pid;
        paquete = crear_paquete(FINALIZAR);
        agregar_a_paquete(paquete, &pid, sizeof(unsigned int));
        semwait(&comunicacionMemoria);
        enviar_paquete_a_servidor(paquete, socketMemoria);
        log_info(logger, "Se envio el proceso %d a la memoria para finalizar", pid);
        //confirmar que llego a memoria
        sem_post(&comunicacionMemoria);

    case DESCONEXION:
        log_info(logger, "Se desconectó el CPU-Dispatch. %d", codOperacion);
        break;

    default:
        log_warning(logger, "Operación desconocida.");
        break;
    }
    */
}

/*

PCB* sacarBloqueado(){
 PCB *pcbSaliente;

    pthread_mutex_lock(&mutexColaBlock);

    pcbSaliente = queue_pop(colaBlock);
    log_info(loggerPlanificacion, "Proceso: [%d] salío de BLOQUEADO.", pcbSaliente->pid);

    pthread_mutex_unlock(&mutexColaBlock);

    return pcbSaliente
}
void *io()
{
    while (1)
    {

        sem_wait(&contadorBloqueados);

        PCB* proceso = queue_peek(colaBloqueados); // Aun sigue en la cola de bloqueado.
        //TODO campo PCB
        //int tiempoBloqueo = proceso->tiempoBloqueadoIO;
        //log_info(loggerPlanificacion, "----------[DISP I/O] Proceso: [%d] ,se bloqueara %d segundos.----------", proceso->pid, tiempoBloqueo / 1000);
        // Bloqueo el proceso.
        int tiempoBloqueoEnMicrosegundos = tiempoBloqueo * 1000;
        usleep(tiempoBloqueoEnMicrosegundos);
        //log_info(loggerPlanificacion, "----------[DISP I/O] Proceso: [%d] ,termino I/O %d segundos.----------", proceso->pid, tiempoBloqueo / 1000);.
        
        proceso = sacarBloqueado();


        agregarAListo(proceso);
    }
}
void planificador_corto_plazo_fifo() {
    // log_info(loggerPlanificacion, "INICIO PLANIFICACION FIFO");
    while (1) {
        sem_wait(&semProcesoReady);
        sem_wait(&semaforoCantidadProcesosExec);

        PCB* procesoEjecutar = list_remove(colaReady,0);


        cambiarEstado(EXEC, procesoEjecutar);

        ejecutar(procesoEjecutar);
    }
}

void planificador_corto_plazo_HRRN() {
    // log_info(loggerPlanificacion, "INICIO PLANIFICACION FIFO");
    while (1) {
        sem_wait(&semProcesoReady);
        sem_wait(&semaforoCantidadProcesosExec);

        PCB* procesoEjecutar = sacarProcesoMayorHRRN();

        cambiarEstado(EXEC, procesoEjecutar);

        ejecutar(procesoEjecutar);
    }
}

PCB* sacarProcesoMayorHRRN(){
  PCB* procesoAEjecutar;
  pthread_mutex_lock(&mutexColaListos);
  list_sort(colaReady, &ordenarSegunCalculoHRRN);
  procesoAEjecutar = list_remove (colaReady,0);
  pthread_mutex_unlock(&mutexColaListos);
  return procesoAEjecutar
}
bool ordenarSegunCalculoHRRN(void* proceso1, void* proceso2){
  return calcularResponseRatio((PCB*)proceso1) > calcularResponseRatio((PCB*)proceso2);
}
float calcularResponseRatio (PCB proceso){
  return ((tiempoAhora()-proceso->llegadaReady)+estimacion(proceso))/estimacion(proceso);
}
float estimacion(PCB proceso){
  float alfa = CONFIG_KERNEL->HRRN_ALFA;
  float estimacionAnterior = proceso->estimadoRafaga;
  int rafagaPrevia = proceso->rafagaRealPrevia * 1000 ;
  float resultado = alfa*rafagaPrevia + (1-alfa) * estimacionAnterior;
  return resultado;
}
int tiempoAhora(){
  return time(NULL);
}
int calcular_tiempo_rafaga_real_anterior(PCB *proceso)
{
    return tiempoAhora() - proceso->llegadaReady;
}
void agregarAListo(PCB* proceso) {
  pthread_mutex_lock(&mutexColaReady);
  list_add(colaReady, proceso);
  //looger
  pthread_mutex_unlock(&mutexColaReady);
}

int sePuedeAgregarMasProcesos() {
  return (CONFIG_KERNEL->GRADO_MAX_MULTIPROGRAMACION > list_size(colaReady) && queue_size(colaNew) > 0)? 1 : 0;
}

void cambiarEstado(estadoProceso estado, PCB* proceso) {
  proceso->estado = estado;
  printf("%d\n", proceso->estado);
}
void agregar_proceso_bloqueado(PCB *procesoBloqueado)
{
    procesoBloqueado->estimacionRafaga = estimacion(procesoBloqueado);
    procesoBloqueado->tiempoRafagaRealAnterior = calcular_tiempo_rafaga_real_anterior(procesoBloqueado);
    pthread_mutex_lock(&mutexColaBloqueados);

    queue_push(colaBloqueados, procesoBloqueado);
    //log_info(loggerPlanificacion, "Proceso: [%d] se movio a BLOQUEADO.", procesoBloqueado->pid);

    pthread_mutex_unlock(&mutexColaBloqueados);

    // Despierto dispositivo I/O
    sem_post(&contadorBloqueados);

    // Despierto al planificador de largo plazo

    sem_post(&despertarPlanificadorLargoPlazo);
}
/*

void enviar_pcb(PCB *proceso, int socketDispatch)
{
    paquete *paquete = crear_paquete(Pcb);

    serializar_pcb(paquete, proceso);

    enviar_paquete_a_servidor(paquete, socketDispatch);

    eliminar_paquete(paquete);
}


bool esProcesoNuevo(Pcb *proceso)
{
    return proceso->escenario->estado == NUEVO;
}



bool sePuedeAgregarMasProcesos() {
    return (cantidad_procesos_memoria() < KERNEL_CONFIG.GRADO_MULTIPROGRAMACION) && (lectura_cola_mutex(colaNuevos, &mutexColaNuevos) > 0 || lectura_cola_mutex(colaSuspendidoListo, &mutexColaSuspendidoListo) > 0);
}

*/
