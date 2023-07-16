#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <commons/config.h>
#include <unistd.h>
#include <utils.h>

t_recursos* recursosFileSystem;
void* copiaBloque;
pthread_mutex_t mutexBloques;
pthread_mutex_t mutexBitMap;
int bytesDelBitarray;
t_bitarray* bitMapBloque;

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
  t_superBloque* superBloque;
  t_config* fileSuperBloque = config_create(recursosFileSystem->configuracion->PATH_SUPERBLOQUE);

  if (fileSuperBloque != NULL) {
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
  bytesDelBitarray = bitsToBytes(recursosFileSystem->superBloque->BLOCK_COUNT);
  ftruncate(fileDescriptor, bytesDelBitarray );
  void* bitmap = mmap(NULL , bytesDelBitarray , PROT_READ | PROT_WRITE , MAP_SHARED , fileDescriptor , 0);
  bitMapBloque = bitarray_create_with_mode((char*)bitmap,bytesDelBitarray, MSB_FIRST);
  recursosFileSystem->bitMap = bitMapBloque;
  msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
  close(fileDescriptor);
}

void cargarBloques(){
  copiaBloque = malloc(recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT);
  int fileDescriptor = open(recursosFileSystem->configuracion->PATH_BLOQUES, O_CREAT | O_RDWR,0664);
  ftruncate(fileDescriptor, recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT);
  void* bloque = mmap(NULL, recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
  //pthread_mutex_lock(&sincro_block);
		memcpy(copiaBloque,bloque,recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT);
		//pthread_mutex_unlock(&sincro_block);
		while(1){ 

			sleep(recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE);
			//pthread_mutex_lock(&sincro_block);
			memcpy(bloque,copiaBloque,recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT);
			//pthread_mutex_unlock(&sincro_block);
			int sincronizacion = msync(bloque, recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT, MS_SYNC);
			if(sincronizacion == -1){
        
        printf("fallo en la sincronizacion\n");
        
        }
			else{
        
        printf("se realizo una sincronizacion exitosa\n");
        
        }
		}

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
  //TODO VER SI COPIA BLOCK VA ACA
  free(recursosFileSystem->conexiones);
  free(recursosFileSystem);
}
