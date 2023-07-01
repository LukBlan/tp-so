#ifndef RECURSOS_H_
#define RECURSOS_H_

  #include <commons/log.h>
  #include <utils.h>
  #include <commons/bitarray.h>
  #include <fcntl.h>
  #include <sys/mman.h>

  typedef struct {
    char* PUERTO_ESCUCHA;
    char* IP_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_SEGMENTO_0;
    int CANT_SEGMENTOS;
    int RETARDO_MEMORIA;
    int RETARDO_COMPACTACION;
    char* ALGOTIRMO_ASIGNACION;
  } t_configuracion;

  typedef struct {
    int socketMemoria;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_log* logger;
    t_conexiones* conexiones;
  } t_recursos;

  extern t_recursos* recursosMemoria;

  void crearRecursosMemoria();
  void cargarLogger(char* pathLogger);
  void cargarConfiguracion(char* pathArchivoConfiguracion);
  void liberarRecursos();

#endif
