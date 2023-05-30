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
    t_configuracion* configuracion;
    t_log* logger;
    int socketCpu;
    int socketMemoria;
  } t_recursos;

  extern t_recursos* recursosCpu;

  void inicializarRecursos(char* pathLogger, char* pathConfiguracion);
  void crearRecursosCpu();
  void cargarConfiguracion(char* pathConfiguracin);
  void cargarLogger(char* pathLogger);
  void liberarRecursos();

#endif
