#ifndef KERNEL_recursos_H
#define KERNEL_recursos_H


  #include <semaphore.h>
  #include <pthread.h>

  #include <commons/log.h>
  #include <commons/collections/list.h>
  #include <commons/collections/queue.h>
#include <commons/config.h>

  #include <estructuras.h>

  typedef struct {
    char* IP_ESCUCHA;
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_FILESYSTEM;
    char* PUERTO_FILESYSTEM;
    char* IP_CPU;
    char* PUERTO_CPU;
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    int ESTIMACION_INICIAL;
    double HRRN_ALFA;
    int GRADO_MAX_MULTIPROGRAMACION;
    t_list* RECURSOS;//es una lista de char
    t_list* INSTANCIAS_RECURSOS;// es una lista de int
  } t_configuracion;

  typedef struct {
    int socketConsola;
    int pid;
  } procesoConsola;

  typedef struct {
    int socketKernel;
    int socketFileSystem;
    int socketMemoria;
    int socketCpu;
    t_list* procesosConsola;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_conexiones* conexiones;
    t_log* logger;
  } t_recursos;

  extern t_recursos* recursosKernel;

  extern t_queue* colaNew;
  extern t_queue* colaBlock;
  extern t_list* colaReady;
  extern PCB* procesoEjecutandose;
  extern t_queue* colaEnd;

  extern pthread_mutex_t mutexNumeroProceso;
  extern pthread_mutex_t mutexProcesoListo;

  extern pthread_mutex_t mutexColaNew;
  extern pthread_mutex_t mutexColaReady;
  extern pthread_mutex_t mutexColaBlock;
  extern pthread_mutex_t mutexColaExec;
  extern pthread_mutex_t mutexColaEnd;
  extern pthread_mutex_t mutexColaArchivos;
  extern pthread_mutex_t operandoConMemoria;

  extern pthread_mutex_t mutex_cola;
  extern pthread_mutex_t mutexcantidadProcesosMemoria;

  extern sem_t semProcesoNew;
  extern sem_t semProcesoReady;
  extern sem_t semaProcesoExec;
  extern sem_t usoMemoria;
  extern sem_t blockCounter;

  extern sem_t largoPlazo;

  extern sem_t semaforoCantidadProcesosExec;
  extern sem_t comunicacionMemoria;

  void crearRecursos();
  t_list* obtenerListaDeInstancias(t_config* fileConfig);
  t_list* obtenerListaDeRecursos(t_config* fileConfig);
  void cargarConfiguracion(char* pathConfiguracion);
  void cargarLogger(char* pathLogger);
  void liberarRecursos();
  void termination_handler(int signum);
  void agarrarSenial();
  void iniciarColas();
  void iniciarSemaforos();

#endif
