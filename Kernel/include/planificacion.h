#ifndef PROCESO_H_
#define PROCESO_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include <estructuras.h>
#include <stdbool.h>

extern t_queue* colaNew;
extern t_queue* colaBlock;
extern t_queue* colaReady;
extern t_queue* colaExec;
extern t_queue* colaEnd;

extern pthread_mutex_t mutexNumeroProceso;
extern pthread_mutex_t mutexProcesoListo;

extern pthread_mutex_t mutexColaNew;
extern pthread_mutex_t mutexColaReady;
extern pthread_mutex_t mutexColaBlock;
extern pthread_mutex_t mutexColaExec;
extern pthread_mutex_t mutexColaEnd;

extern pthread_mutex_t mutex_cola;
extern pthread_mutex_t mutexcantidadProcesosMemoria;

extern sem_t semProcesoNew;
extern sem_t semProcesoReady;
extern sem_t semaProcesoExec;

extern sem_t blockCounter;

extern sem_t largoPlazo;

extern sem_t semaforoCantidadProcesosExec;
extern sem_t comunicacionMemoria;

void iniciarColas();
void iniciarSemaforos();

void comenzarPlanificador();

void planificador_largo_plazo();

void agregarAListo(PCB* proceso);

int sePuedeAgregarMasProcesos();
void cambiarEstado(estadoProceso estado, PCB* proceso);


#endif
