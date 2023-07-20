#include <commons/config.h>
#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

t_queue* colaNew;
t_list *colaReady;
PCB* procesoEjecutandose;
t_queue* colaBlock;
t_queue* colaEnd;

pthread_mutex_t mutexNumeroProceso;
pthread_mutex_t mutexProcesoListo;

pthread_mutex_t mutexColaNew;
pthread_mutex_t mutexColaReady;
pthread_mutex_t mutexColaBlock;
pthread_mutex_t mutexColaExec;
pthread_mutex_t mutexColaEnd;
pthread_mutex_t mutexColaArchivos;

pthread_mutex_t mutex_cola;
pthread_mutex_t mutexcantidadProcesosMemoria;

sem_t semProcesoNew;
sem_t semProcesoReady;
sem_t semaProcesoExec;

sem_t blockCounter;

sem_t largoPlazo;

sem_t semaforoCantidadProcesosExec;
sem_t comunicacionMemoria;

t_recursos* recursosKernel;

void crearRecursos() {
  recursosKernel = malloc(sizeof(t_recursos));
  recursosKernel->configuracion = NULL;
  recursosKernel->conexiones = malloc(sizeof(t_conexiones));
  recursosKernel->conexiones->procesosConsola = list_create();
  recursosKernel->conexiones->socketCpu = -1;
  recursosKernel->conexiones->socketFileSystem = -1;
  recursosKernel->conexiones->socketMemoria = -1;
}

t_list* obtenerListaDeInstancias(t_config* fileConfig) {
  t_list* listaInstancias = list_create();
  char** instanciasRecursos = config_get_array_value(fileConfig,"INSTANCIAS_RECURSOS");
  int i = 0;
  while(instanciasRecursos[i] != NULL) {
    list_add(listaInstancias, atoi(instanciasRecursos[i]));
    i++;
  }
  free(instanciasRecursos);
  return listaInstancias;
}

t_list* obtenerListaDeRecursos(t_config* fileConfig) {
  t_list* listaRecursos = list_create();
    char** recursos = config_get_array_value(fileConfig,"RECURSOS");
    int i = 0;
    while(recursos[i] != NULL) {
      list_add(listaRecursos, recursos[i]);
      i++;
    }
    free(recursos);
    return listaRecursos;
}

void mostrarConfig() {
  puts("1");
  t_configuracion* config = recursosKernel->configuracion;
  t_list* listaRecursos = config->RECURSOS;
  t_list* listaInstancias = config->INSTANCIAS_RECURSOS;
  puts("2");
  printf("%d = %d\n", listaRecursos->elements_count, listaInstancias->elements_count);
  puts("3");
  for (int i = 0; i < listaRecursos->elements_count; i++) {
    printf("Recurso: %s cantidad: %d\n", list_get(listaRecursos, i), list_get(listaInstancias, i));
  }

}

void cargarConfiguracion(char* pathConfiguracion) {
  t_configuracion* config;
  t_config* fileConfig = config_create(pathConfiguracion);

  if (fileConfig != NULL) {
    config = malloc(sizeof(t_configuracion));
    config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
    config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
    config->PUERTO_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_MEMORIA"));
    config->IP_FILESYSTEM = string_duplicate(config_get_string_value(fileConfig, "IP_FILESYSTEM"));
    config->PUERTO_FILESYSTEM = string_duplicate(config_get_string_value(fileConfig, "PUERTO_FILESYSTEM"));
    config->IP_CPU = string_duplicate(config_get_string_value(fileConfig, "IP_CPU"));
    config->PUERTO_CPU = string_duplicate(config_get_string_value(fileConfig, "PUERTO_CPU"));
    config->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_ESCUCHA"));
    config->ALGORITMO_PLANIFICACION = string_duplicate(config_get_string_value(fileConfig,"ALGORITMO_PLANIFICACION"));
    config->ESTIMACION_INICIAL = config_get_int_value(fileConfig,"ESTIMACION_INICIAL");
    config->HRRN_ALFA = config_get_int_value(fileConfig,"HRRN_ALFA");
    config->GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(fileConfig,"GRADO_MAX_MULTIPROGRAMACION");
    config->RECURSOS = obtenerListaDeRecursos(fileConfig);
    config->INSTANCIAS_RECURSOS = obtenerListaDeInstancias(fileConfig);

  } else {
    log_error(recursosKernel->logger, "No se pudo Encontrar el Archivo de configuracion");
    liberarRecursos();
    exit(-1);
  }

  config_destroy(fileConfig);
  recursosKernel->configuracion = config;
  mostrarConfig();
}

void cargarLogger(char* pathLogger) {
  recursosKernel->logger = log_create(pathLogger, "Kernel", true, LOG_LEVEL_INFO);
}

void liberarRecursos() {
  if (recursosKernel->logger != NULL) {
    free(recursosKernel->configuracion->IP_ESCUCHA);
    free(recursosKernel->configuracion->IP_MEMORIA);
    free(recursosKernel->configuracion->PUERTO_MEMORIA);
    free(recursosKernel->configuracion->IP_FILESYSTEM);
    free(recursosKernel->configuracion->PUERTO_FILESYSTEM);
    free(recursosKernel->configuracion->IP_CPU);
    free(recursosKernel->configuracion->PUERTO_CPU);
    free(recursosKernel->configuracion->PUERTO_ESCUCHA);
    free(recursosKernel->configuracion->ALGORITMO_PLANIFICACION);
    free(recursosKernel->configuracion);
  }

  if (recursosKernel->conexiones->socketCpu > 0) {
	log_info(recursosKernel->logger, "Cerrando Servidor kernel...");
    close(recursosKernel->conexiones->socketCpu);
  }

  if (recursosKernel->conexiones->socketFileSystem > 0) {
    close(recursosKernel->conexiones->socketFileSystem);
  }

  if (recursosKernel->conexiones->socketKernel > 0) {
    close(recursosKernel->conexiones->socketKernel);
  }

  if (recursosKernel->conexiones->socketMemoria > 0) {
      close(recursosKernel->conexiones->socketMemoria);
    }

  if (recursosKernel->logger != NULL) {
    log_destroy(recursosKernel->logger);
  }

  list_destroy(recursosKernel->conexiones->procesosConsola);
  free(recursosKernel->conexiones);
  free(recursosKernel);
}
void termination_handler(int signum){
	 liberarRecursos();
	 exit(-1);
 }

void agarrarSenial(){
	struct sigaction nuevaAccion;
	nuevaAccion.sa_handler = termination_handler;
	sigaction(SIGTERM,&nuevaAccion, NULL);
	}


void iniciarColas() {
  colaNew = queue_create();
  colaReady = list_create();
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
  pthread_mutex_init(&mutexColaArchivos, NULL);

  sem_init(&semProcesoNew, 0, 0);
  sem_init(&semProcesoReady, 0, 0);
  sem_init(&blockCounter, 0, 0);
  sem_init(&semaforoCantidadProcesosExec, 0, 1);
  sem_init(&largoPlazo, 0, 0);
}
