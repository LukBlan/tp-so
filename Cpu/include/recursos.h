#ifndef CPU_RECURSOS_H
#define CPU_RECURSOS_H

  #include <commons/log.h>
  #include <estructuras.h>

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
    t_registros registros;
  } t_recursos;

  extern t_recursos* recursosCpu;

  void crearRecursosCpu();
  void cargarConfiguracion(char* pathConfiguracin);
  void cargarLogger(char* pathLogger);
  void liberarRecursos();
  void agarrarSenial();
  void termination_handler(int signum);
  void llamarAMmu(int direccion_logica);

#endif
