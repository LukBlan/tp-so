#ifndef CPU_RECURSOS_H_
#define CPU_RECURSOS_H_

  #include <commons/log.h>

  typedef struct {
    int RETARDO_INSTRUCCION;
    char* IP_MEMORIA;
    char* IP_ESCUCHA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA;
    int TAM_MAX_SEGMENTO;
  } t_configuracion;

  typedef struct {
    int socketCpu;
    int socketMemoria;
    int socketKernel;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_conexiones* conexiones;
    t_log* logger;
  } t_recursos;

  extern t_recursos* recursosCpu;

  void crearRecursosCpu();
  void cargarConfiguracion(char* pathConfiguracin);
  void cargarLogger(char* pathLogger);
  void liberarRecursos();

#endif
