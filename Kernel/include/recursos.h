#ifndef KERNEL_recursos_H_
#define KERNEL_recursos_H_

  #include <commons/log.h>

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
    int HRRN_ALFA;
    int GRADO_MAX_MULTIPROGRAMACION;
    int RECURSOS;//es una lista de char
    int INSTANCIAS_RECURSOS;// es una lista de int
  } t_configuracion;

  typedef struct {
    int socketFileSystem;
    int socketMemoria;
    int socketCpu;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_conexiones* conexiones;
    t_log* logger;
  } t_recursos;

  extern t_recursos* recursosKernel;

  void crearRecursos();
  void cargarConfiguracion(char* pathConfiguracion);
  void cargarLogger(char* pathLogger);

#endif
