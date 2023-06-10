#include <commons/log.h>
#include <recursos.h>
#include <stdlib.h>
#include <consola_conexiones.h>
#include <unistd.h>

t_recursos* recursosConsola;

void crearRecursosConsola() {
  recursosConsola = malloc(sizeof(t_recursos));
  recursosConsola->conexiones = malloc(sizeof(t_conexiones));
  recursosConsola->configuracion = NULL;
  recursosConsola->logger = NULL;
  recursosConsola->archivoPseudoCodigo = NULL;
  recursosConsola->conexiones->socketKernel = -1;
}

void cargarLogger(char* pathLogger) {
  recursosConsola->logger = log_create(pathLogger, "Consola", true, LOG_LEVEL_INFO);
}

void cargarConfiguracion(char* pathConfiguracion) {
  t_config *archivoConfig = config_create(pathConfiguracion);

  if (archivoConfig != NULL) {
    t_configuracion* config = malloc(sizeof(t_configuracion));
    config->IP_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "IP_KERNEL"));
    config->PUERTO_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "PUERTO_KERNEL"));
    recursosConsola->configuracion = config;
    config_destroy(archivoConfig);
  } else {
    log_error(recursosConsola->logger, "No se pudo Encontrar el Archivo de configuracion");
    liberarRecursos();
    exit(-1);
  }
}

void cargarPseudoCodigo(char* pathPseudoCodigo) {
  FILE* archivoPseudoCodigo = fopen(pathPseudoCodigo, "r");
  if (!archivoPseudoCodigo) {
    log_error(recursosConsola->logger, "No se pudo encontrar el archivo con pseudocodigo.");
    liberarRecursos();
    exit(-2);
  }

  recursosConsola->archivoPseudoCodigo = archivoPseudoCodigo;
}

void liberarRecursos() {
  if (recursosConsola->configuracion != NULL) {
    free(recursosConsola->configuracion->IP_KERNEL);
    free(recursosConsola->configuracion->PUERTO_KERNEL);
    free(recursosConsola->configuracion);
  }

  if (recursosConsola->conexiones->socketKernel > 0) {
      log_info(recursosConsola->logger, "Cerrando conexion con el Servidor Kernel...");
      close(recursosConsola->conexiones->socketKernel);
  }

  if (!recursosConsola->archivoPseudoCodigo) {
    fclose(recursosConsola->archivoPseudoCodigo);
  }

  if (recursosConsola->logger != NULL) {
    log_destroy(recursosConsola->logger);
  }

  free(recursosConsola->conexiones);
  free(recursosConsola);
}
