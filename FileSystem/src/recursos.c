#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <commons/config.h>
#include <unistd.h>
#include <utils.h>
#include <dirent.h>
#include <signal.h>

t_recursos* recursosFileSystem;
void* copiaBloque;
pthread_mutex_t mutexBloques;
pthread_mutex_t mutexBitMap;
int bytesDelBitarray;
t_bitarray* bitMapBloque;
t_list* listaDeFCB;
void* bitmapMapeado;
void* bloque;

void crearRecursosFileSystem(char* pathLogger, char* pathConfiguracion) {
  recursosFileSystem = malloc(sizeof(t_recursos));
  recursosFileSystem->conexiones = malloc(sizeof(t_conexiones));
  recursosFileSystem->configuracion = NULL;
  recursosFileSystem->superBloque = NULL;
  recursosFileSystem->logger = NULL;
  recursosFileSystem->conexiones->socketFileSystem = -1;
  recursosFileSystem->conexiones->socketMemoria = -1;
}
void generarListaDeFCB(){
  listaDeFCB = list_create();
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
    config->PATH_FCB = string_duplicate(config_get_string_value(fileConfig, "PATH_FCB"));
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
  int fileDescriptor = open (recursosFileSystem->configuracion->PATH_BITMAP, O_RDWR);
  bytesDelBitarray = bitsToBytes(recursosFileSystem->superBloque->BLOCK_COUNT);
  ftruncate(fileDescriptor, bytesDelBitarray );
  bitmapMapeado = mmap(NULL , bytesDelBitarray , PROT_READ | PROT_WRITE , MAP_SHARED , fileDescriptor , 0);
  bitMapBloque = bitarray_create_with_mode(bitmapMapeado,bytesDelBitarray, LSB_FIRST);
  //recursosFileSystem->bitMap = bitMapBloque;
  //msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
  close(fileDescriptor);
}

void cargarBloques() {
  int tamanioTotalBloques = recursosFileSystem->superBloque->BLOCK_SIZE * recursosFileSystem->superBloque->BLOCK_COUNT;
  copiaBloque = malloc(tamanioTotalBloques);
  int fileDescriptor = open(recursosFileSystem->configuracion->PATH_BLOQUES, O_CREAT | O_RDWR,0664);
  ftruncate(fileDescriptor, tamanioTotalBloques);
  bloque = mmap(NULL, tamanioTotalBloques, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);;
  //msync(bloque, tamanioTotalBloques, MS_SYNC);
}

void iniciarFCBExistente(){
	DIR *directorioFCB = opendir(recursosFileSystem->configuracion->PATH_FCB);
	struct dirent *fcb;

	if(directorioFCB == NULL){
    puts("No existe directorio");
		exit(1);
	}

	while((fcb = readdir(directorioFCB)) != NULL){
		if (strcmp(fcb->d_name, ".") == 0 || strcmp(fcb->d_name, "..") == 0){
			continue;
		}

		configArchivo* archivo = malloc(sizeof(configArchivo));
		archivo->nombre_archivo = malloc(strlen(fcb->d_name));
		strcpy(archivo->nombre_archivo, fcb->d_name);

		char* path_archivo = malloc(strlen(recursosFileSystem->configuracion->PATH_FCB) + strlen(fcb->d_name));
		strcpy(path_archivo, recursosFileSystem->configuracion->PATH_FCB);
		strcat(path_archivo, fcb->d_name);
		archivo->configFCB = config_create(path_archivo);

		list_add(listaDeFCB, archivo);
	}

	closedir(directorioFCB);
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
	log_info(recursosFileSystem->logger, "Cerrando Servidor de FileSystem..");
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

void inicializarSemaforos() {
  pthread_mutex_init(&mutexBloques, NULL);
  pthread_mutex_init(&mutexBitMap, NULL);
}

void termination_handler(int signum){
	 liberarRecursos();
	 exit(-1);
 }

void agarrarSenial(){
	struct sigaction nuevaAccion;
	nuevaAccion.sa_handler = termination_handler;
	sigaction(SIGTERM,&nuevaAccion, NULL);
	}
