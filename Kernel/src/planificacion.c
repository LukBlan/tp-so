#include <planificacion.h>
#include <estructuras.h>
#include <kernel_config.h>

t_queue* colaNew;
t_queue* colaReady;
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
  colaReady = queue_create();
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

void comenzarPlanificador() {
  pthread_t hilo_largo_plazo;
  pthread_t hiloCortoPlazo;

  pthread_create(&hilo_largo_plazo, NULL, (void*) planificador_largo_plazo, NULL);
  pthread_detach(hilo_largo_plazo);

  if (strcmp(CONFIG_KERNEL->ALGORITMO_PLANIFICACION, "FIFO") == 0) {
    pthread_create(&hiloCortoPlazo, NULL, planificador_corto_plazo_fifo, NULL);
    pthread_detach(hiloCortoPlazo);
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

    if (sePuedeAgregarMasProcesos()) {
      PCB *procesoSaliente = queue_pop(colaNew);
      cambiarEstado(READY, procesoSaliente);
      agregarAListo(procesoSaliente);
    }
  }
}

void planificador_corto_plazo_fifo() {
    // log_info(loggerPlanificacion, "INICIO PLANIFICACION FIFO");
    while (1) {
        sem_wait(&semProcesoReady);
        sem_wait(&semaforoCantidadProcesosExec);

        PCB* procesoEjecutar = queue_pop(colaReady);

        cambiarEstado(EXEC, procesoEjecutar);

        ejecutar(procesoEjecutar);
    }
}


void ejecutar(PCB *proceso) {
  //serializar la pcb
  //enviarla a cpu
  // se queda esperando el codOperacion
  // con ese codigo hace algo

  /*
    enviar_pcb(proceso, socketCPU);
    //log_info(logger, "Envio proceso con PID: [%d] por CPU-Dispatch.", proceso->pid);

    op_code codOperacion = obtener_codigo_operacion(socketCPU);

    Pcb *procesoRecibido;

    switch (codOperacion) {
    case PCB:
        procesoRecibido = deserializar_pcb(socketCPU);
        log_info(logger, "Recibi proceso con PID: %d de CPU-Dispatch.", proceso->pid);
        manejar_proceso_recibido(procesoRecibido, socketCPU);
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


void agregarAListo(PCB* proceso) {
  pthread_mutex_lock(&mutexColaReady);
  queue_push(colaReady, proceso);
  //looger
  pthread_mutex_unlock(&mutexColaReady);
}

int sePuedeAgregarMasProcesos() {
  return (CONFIG_KERNEL->GRADO_MAX_MULTIPROGRAMACION > queue_size(colaReady) && queue_size(colaNew) > 0)? 1 : 0;
}

void cambiarEstado(estadoProceso estado, PCB* proceso) {
  proceso->estado = estado;
  printf("%d\n", proceso->estado);
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

bool sePuedeAgregarMasProcesos()
{
    return (cantidad_procesos_memoria() < KERNEL_CONFIG.GRADO_MULTIPROGRAMACION) && (lectura_cola_mutex(colaNuevos, &mutexColaNuevos) > 0 || lectura_cola_mutex(colaSuspendidoListo, &mutexColaSuspendidoListo) > 0);
}
*/
