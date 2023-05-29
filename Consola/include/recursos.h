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
    t_configuracion* configuracion;
    t_log* logger;
    char* pathPseudoCodigo;
  } t_recursos;

  extern t_recursos* recursosConsola;

  t_recursos* generarLogger(char*);
  void obtenerConfiguracion(t_recursos*, char*);
  void obtenerPseudoCodigo(t_recursos*, char*);
  void configuracion_consola(t_config *config);
  void liberarRecursos(t_recursos* recursosConsola);

#endif
