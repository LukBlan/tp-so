#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <commons/config.h>
#include <unistd.h>

t_recursos* recursosFileSystem;

void crearRecursosFileSystem(char* pathLogger, char* pathConfiguracion) {
  recursosFileSystem = malloc(sizeof(t_recursos));
  recursosFileSystem->conexiones = malloc(sizeof(t_conexiones));
  recursosFileSystem->configuracion = NULL;
  recursosFileSystem->superBloque = NULL;
  recursosFileSystem->logger = NULL;
  recursosFileSystem->conexiones->socketFileSystem = -1;
  recursosFileSystem->conexiones->socketMemoria = -1;
}

void cargarConfiguracion(char* pathConfiguracin) {
  t_configuracion* config;
  t_config* fileConfig = config_create(pathConfiguracin);

  if (fileConfig != NULL) {
    config = malloc(sizeof(t_configuracion));
    config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
    config->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_ESCUCHA"));
    config->PUERTO_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_MEMORIA"));
    config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
    config->PATH_SUPERBLOQUE = string_duplicate(config_get_string_value(fileConfig, "PATH_SUPERBLOQUE"));
    config->PATH_BITMAP = string_duplicate(config_get_string_value(fileConfig, "PATH_BITMAP"));
    config->PATH_BLOQUES = string_duplicate(config_get_string_value(fileConfig, "PATH_BLOQUES"));
    config->RETARDO_ACCESO_BLOQUE = config_get_int_value(fileConfig, "RETARDO_ACCESO_BLOQUE");
  } else {
    log_error(recursosFileSystem->logger, "No se pudo Encontrar el Archivo de configuracion");
    liberarRecursos();
    exit(-1);
  }

  config_destroy(fileConfig);
  recursosFileSystem->configuracion = config;
}

void cargarSuperbloque() {
  fileSuperBloque* superBloque;
  t_config* fileSuperBloque = config_create(recursosFileSystem->configuracion->PATH_SUPERBLOQUE);

  if (fileConfig != NULL) {
    superBloque = malloc(sizeof(fileSuperBloque));
    superBloque->BLOCK_SIZE = config_get_int_value(fileSuperBloque, "BLOCK_SIZE");
    superBloque->BLOCK_COUNT = config_get_int_value(fileSuperBloque, "BLOCK_COUNT");
  } else {
    log_error(recursosFileSystem->logger, "No se pudo Encontrar el Archivo de superBloque");
    liberarRecursos();
    exit(-1);
  }

  config_destroy(fileSuperBloque);
  recursosFileSystem->superBloque = superBloque;
}

void cargarBitMap() {
  int fileDescriptor = open (recursosFileSystem->configuracion->PATH_BITMAP, O_CREAT | O_RDWR,0664);
  int bytesDelBitarray = bitAByte(recursosFileSystem->configuracion->PATH_SUPERBLOQUE->BLOCK_COUNT);
  void* bitmap = mmap(NULL , bytesDelBitarray , PROT_READ | PROT_WRITE , MAP_SHARED , fileDescriptor , 0);
  t_bitarray* bitMapBloque = bitarray_create_with_mode((char*)bitmap,bytesDelBitarray, MSB_FIRST);
  recursosFileSystem->bitMap = bitMapBloque;
  msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
  close(fileDescriptor);

}


void cargarLogger(char* pathLogger) {
  recursosFileSystem->logger = log_create(pathLogger, "FileSystem", 1, LOG_LEVEL_INFO);
}

void liberarRecursos() {
  if (recursosFileSystem->configuracion != NULL) {
    free(recursosFileSystem->configuracion->PUERTO_ESCUCHA);
    free(recursosFileSystem->configuracion->PUERTO_MEMORIA);
    free(recursosFileSystem->configuracion->IP_ESCUCHA);
    free(recursosFileSystem->configuracion->IP_MEMORIA);
    free(recursosFileSystem->configuracion->PATH_SUPERBLOQUE);
    free(recursosFileSystem->configuracion->PATH_BITMAP);
    free(recursosFileSystem->configuracion->PATH_BLOQUES);
    free(recursosFileSystem->configuracion);
  }

  if (recursosFileSystem->conexiones->socketFileSystem > 0) {
    close(recursosFileSystem->conexiones->socketFileSystem);
  }

  if (recursosFileSystem->conexiones->socketMemoria > 0) {
     close(recursosFileSystem->conexiones->socketMemoria);
   }

  if (recursosFileSystem->logger != NULL) {
    log_destroy(recursosFileSystem->logger);
  }
  //TODO FREE BITMAP Y SUPERBLOQUE
  free(recursosFileSystem->conexiones);
  free(recursosFileSystem);
}
