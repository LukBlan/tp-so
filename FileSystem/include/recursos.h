#ifndef RECURSOS_H_
#define RECURSOS_H_

  #include <commons/log.h>
  #include <commons/bitarray.h>
 #include <commons/config.h>
  #include <utils.h>
  #include <fcntl.h>
  #include <sys/mman.h>
#include <instrucciones.h>

  typedef struct {
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_ESCUCHA;
    char* PUERTO_ESCUCHA;
    char* PATH_SUPERBLOQUE;
    char* PATH_BITMAP;
    char* PATH_BLOQUES;
    char* PATH_FCB;
    int RETARDO_ACCESO_BLOQUE;
  } t_configuracion;
  typedef struct {
    int BLOCK_SIZE;
    int BLOCK_COUNT;
  } t_superBloque;

  typedef struct {
    char* nombre_archivo;
    int file_size;
    uint32_t punteroDirecto;
    uint32_t punteroIndirecto;
  } FCB;

typedef struct{
	char* nombre_archivo;
	t_config* configFCB;
} configArchivo;

  typedef struct {
    int socketMemoria;
    int socketFileSystem;
  } t_conexiones;

  typedef struct {
    t_configuracion* configuracion;
    t_log* logger;
    t_conexiones* conexiones;
    t_superBloque* superBloque;
    t_bitarray* bitMap;
  } t_recursos;

  extern t_recursos* recursosFileSystem;
  extern void* copiaBloque;
  extern pthread_mutex_t mutexBloques;
  extern pthread_mutex_t mutexBitMap;
  extern int bytesDelBitarray;
  extern t_bitarray* bitMapBloque;
  extern t_list* listaDeFCB;
  extern void* bitmapMapeado;
  extern void* bloque;
  
  void crearRecursosFileSystem();
  void generarListaDeFCB();
  void cargarConfiguracion(char* pathConfiguracin);
  void cargarSuperbloque();
  void cargarBitMap();
  void cargarBloques();
  void iniciarFCBExistente();
  void cargarLogger(char* pathLogger);
  void liberarRecursos();
  void inicializarSemaforos();
  void termination_handler(int signum);
  void agarrarSenial();

#endif
