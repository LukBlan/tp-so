#include <commons/config.h>
#include <commons/string.h>
#include <recursos.h>
#include <stdlib.h>
#include <commons/config.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
t_recursos* recursosCpu;

void crearRecursosCpu() {
  recursosCpu = malloc(sizeof(t_recursos));
  recursosCpu->conexiones = malloc(sizeof(t_conexiones));
  recursosCpu->configuracion = NULL;
  recursosCpu->logger = NULL;
  recursosCpu->conexiones->socketCpu = -1;
  recursosCpu->conexiones->socketMemoria = -1;
}

void cargarConfiguracion(char* pathConfiguracin) {
  t_configuracion* config;
  t_config* fileConfig = config_create(pathConfiguracin);

  if (fileConfig != NULL) {
    config = malloc(sizeof(t_configuracion));
    config->IP_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "IP_MEMORIA"));
    config->IP_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "IP_ESCUCHA"));
    config->TAM_MAX_SEGMENTO = config_get_int_value(fileConfig, "TAM_MAX_SEGMENTO");
    config->PUERTO_ESCUCHA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_ESCUCHA"));
    config->PUERTO_MEMORIA = string_duplicate(config_get_string_value(fileConfig, "PUERTO_MEMORIA"));
    config->RETARDO_INSTRUCCION = config_get_int_value(fileConfig, "RETARDO_INSTRUCCION");
  } else {
    log_error(recursosCpu->logger, "No se pudo Encontrar el Archivo de configuracion");
    liberarRecursos();
    exit(-1);
  }

  config_destroy(fileConfig);
  recursosCpu->configuracion = config;
}

void cargarLogger(char* pathLogger) {
  recursosCpu->logger = log_create(pathLogger, "CPU", 1, LOG_LEVEL_INFO);;
}

void darNumeroSegmentoMMU(int direccion_logica) {
    int tamanioMaxSegmento = recursosCpu->configuracion->TAM_MAX_SEGMENTO;
    int numero_segmento = floor(direccion_logica / tamanioMaxSegmento);
    return numero_segmento;
}
int darDesplazamientoMMU(int direccion_logica){
    int tamanioMaxSegmento = recursosCpu->configuracion->TAM_MAX_SEGMENTO;
    int numero_segmento = floor(direccion_logica / tamanioMaxSegmento);
    int desplazamiento_segmento = direccion_logica - (numero_segmento * tamanioMaxSegmento);
    return desplazamiento_segmento;
}
void liberarRecursos() {
  if (recursosCpu != NULL) {
    free(recursosCpu->configuracion->PUERTO_ESCUCHA);
    free(recursosCpu->configuracion->PUERTO_MEMORIA);
    free(recursosCpu->configuracion->IP_ESCUCHA);
    free(recursosCpu->configuracion->IP_MEMORIA);
    free(recursosCpu->configuracion);
  }

  if (recursosCpu->conexiones->socketCpu > 0) {
	log_info(recursosCpu->logger, "Cerrando Servidor de Cpu...");
    close(recursosCpu->conexiones->socketCpu);
  }

  if (recursosCpu->conexiones->socketMemoria > 0) {
     close(recursosCpu->conexiones->socketMemoria);
   }

  if (recursosCpu->conexiones->socketKernel > 0) {
       close(recursosCpu->conexiones->socketKernel);
     }

  if (recursosCpu->logger != NULL) {
    log_destroy(recursosCpu->logger);
  }

  free(recursosCpu->conexiones);
  free(recursosCpu);
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
