#include <recursos.h>
#include <stdlib.h>
#include <consola_conexiones.h>
#include <unistd.h>

t_recursos* recursosConsola;

void inicializarRecursos(char* pathLoger, char* pathConfiguracion, char* pathPseudoCodigo) {
  recursosConsola = malloc(sizeof(t_recursos));
  recursosConsola->configuracion = NULL;
  recursosConsola->logger = NULL;
  recursosConsola->pathPseudoCodigo = NULL;
  recursosConsola->socketKernel = -1;
  cargarLogger(pathLoger);
  cargarConfiguracion(pathConfiguracion);
  cargarPseudoCodigo(pathPseudoCodigo);
  generarConexionConKernel();
}

void cargarLogger(char* pathLogger) {
  recursosConsola->logger = log_create(pathLogger, "Consola", true, LOG_LEVEL_INFO);
}

void cargarConfiguracion(char* pathConfiguracion) {
  t_config *archivoConfig = config_create(pathConfiguracion);
  t_configuracion* config = malloc(sizeof(t_configuracion));
  config->IP_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "IP_KERNEL"));
  config->PUERTO_KERNEL = string_duplicate(config_get_string_value(archivoConfig, "PUERTO_KERNEL"));
  recursosConsola->configuracion = config;
  config_destroy(archivoConfig);
}

void cargarPseudoCodigo(char* pathPseudoCodigo) {
  if (pathPseudoCodigo == NULL) {
    log_error(recursosConsola->logger, "No se pudo abrir el archivo.");
    liberarRecursos();
    exit(-2);
  }
  recursosConsola->pathPseudoCodigo = pathPseudoCodigo;
}

void liberarRecursos() {
  if (recursosConsola->configuracion != NULL) {
    free(recursosConsola->configuracion->IP_KERNEL);
    free(recursosConsola->configuracion->PUERTO_KERNEL);
    free(recursosConsola->configuracion);
  }

  if (recursosConsola->socketKernel > 0) {
      log_info(recursosConsola->logger, "Cerrando conexion con el Servidor Kernel...");
      close(recursosConsola->socketKernel);
    }

  if (recursosConsola->logger != NULL) {
    log_destroy(recursosConsola->logger);
  }

  free(recursosConsola);
}
