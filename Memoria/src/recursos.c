#include <commons/config.h>
#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
t_recursos* recursosMemoria;

void crearRecursosMemoria() {
  recursosMemoria = malloc(sizeof(t_recursos));
  recursosMemoria->conexiones = malloc(sizeof(t_conexiones));
  recursosMemoria->configuracion = NULL;
  recursosMemoria->logger = NULL;
  recursosMemoria->conexiones->socketMemoria = -1;
}

void cargarLogger(char* pathLogger) {
  recursosMemoria->logger = log_create(pathLogger, "Memoria", true, LOG_LEVEL_INFO);
}

void cargarConfiguracion(char* pathArchivoConfiguracion) {
  t_configuracion* configuracion;
  t_config* archivoConfiguracion = config_create(pathArchivoConfiguracion);

  if (archivoConfiguracion != NULL) {
    configuracion = malloc(sizeof(t_configuracion));
    configuracion->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(archivoConfiguracion, "PUERTO_ESCUCHA"));
    configuracion->IP_ESCUCHA = string_duplicate(config_get_string_value(archivoConfiguracion, "IP_ESCUCHA"));
    configuracion->TAM_MEMORIA = config_get_int_value(archivoConfiguracion, "TAM_MEMORIA");
    configuracion->TAM_SEGMENTO_0 = config_get_int_value(archivoConfiguracion, "TAM_SEGMENTO_0");
    configuracion->CANT_SEGMENTOS = config_get_int_value(archivoConfiguracion, "CANT_SEGMENTOS");
    configuracion->RETARDO_MEMORIA= config_get_int_value(archivoConfiguracion, "RETARDO_MEMORIA");
    configuracion->RETARDO_COMPACTACION = config_get_int_value(archivoConfiguracion, "RETARDO_COMPACTACION");
    configuracion->ALGOTIRMO_ASIGNACION = string_duplicate(config_get_string_value(archivoConfiguracion, "ALGORITMO_ASIGNACION"));
  } else {
    log_error(recursosMemoria->logger, "No se pudo Encontrar el Archivo de configuracion");
    liberarRecursos();
    exit(-1);
  }

  config_destroy(archivoConfiguracion);
  recursosMemoria->configuracion = configuracion;
}

void liberarRecursos() {
  if (recursosMemoria->configuracion != NULL) {
    free(recursosMemoria->configuracion->PUERTO_ESCUCHA);
    free(recursosMemoria->configuracion->IP_ESCUCHA);
    free(recursosMemoria->configuracion->ALGOTIRMO_ASIGNACION);
    free(recursosMemoria->configuracion);
  }

  if (recursosMemoria->conexiones->socketMemoria > 0) {
    log_info(recursosMemoria->logger, "Cerrando Servidor Memoria...");
    close(recursosMemoria->conexiones->socketMemoria);
  }

  if (recursosMemoria->logger != NULL) {
    log_destroy(recursosMemoria->logger);
  }

  free(recursosMemoria->conexiones);
  free(recursosMemoria);
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


