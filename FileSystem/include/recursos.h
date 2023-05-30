#ifndef RECURSOS_H_
#define RECURSOS_H_

  #include <commons/log.h>

  typedef struct {
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_ESCUCHA;
    char* PUERTO_ESCUCHA;
    char* PATH_SUPERBLOQUE;
    char* PATH_BITMAP;
    char* PATH_BLOQUES;
    int RETARDO_ACCESO_BLOQUE;
  } t_configuracion;

  typedef struct {
    int socketMemoria;
    int socketFileSystem;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_log* logger;
    t_conexiones* conexiones;
  } t_recursos;

  extern t_recursos* recursosFileSystem;

  void crearRecursosFileSystem();
  void cargarLogger(char* pathLogger);
  void cargarConfiguracion(char* pathArchivoConfiguracion);
  void liberarRecursos();

#endif
