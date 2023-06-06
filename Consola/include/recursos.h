#ifndef RECURSOS_H
#define RECURSOS_H

  #include <commons/log.h>
  #include <commons/config.h>
  #include <commons/string.h>

  typedef struct {
    char *IP_KERNEL;
    char *PUERTO_KERNEL;
  } t_configuracion;

  typedef struct {
    int socketKernel;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_conexiones* conexiones;
    t_log* logger;
    FILE* archivoPseudoCodigo;
    int socketKernel;
  } t_recursos;

  extern t_recursos* recursosConsola;

  void crearRecursosConsola();
  void cargarLogger(char* pathLogger);
  void cargarConfiguracion(char* pathConfiguracion);
  void cargarPseudoCodigo(char* pathPseudoCodigo);
  void configuracion_consola(t_config* config);
  void liberarRecursos();

#endif
