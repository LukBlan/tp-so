#include <commons/string.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

void cargarConexiones() {
  conectarConMemoria();
  generarServidorFileSystem();
}

void conectarConMemoria() {
  t_configuracion* config = recursosFileSystem->configuracion;
  t_log* logger = recursosFileSystem->logger;

  log_info(logger, "Conectando con el Servidor Memoria...");
  int socketMemoria = crearConexionServidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);

  if (socketMemoria < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Memoria no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosFileSystem->conexiones->socketMemoria = socketMemoria;
}

void generarServidorFileSystem() {
  t_configuracion* config = recursosFileSystem->configuracion;
  t_log* logger = recursosFileSystem->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor FileSystem...");
  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor FileSystem.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Socket del Servidor FileSystem Creado Exitosamente.");
  recursosFileSystem->conexiones->socketFileSystem = socketServidor;
}

void montarServidor() {
  t_log* logger = recursosFileSystem->logger;
  int socketServidor = recursosFileSystem->conexiones->socketFileSystem;

  log_info(logger, "Servidor FileSystem se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
  close(socketServidor);
}

void realizarHandshakeMemoria() {
  t_log* logger = recursosFileSystem->logger;
  int socketMemoria = recursosFileSystem->conexiones->socketMemoria;

  log_info(logger, "Realizando Handshake con Memoria");
  if (realizarHandshake(logger, socketMemoria) < 0) {
    liberarRecursos();
    exit(-1);
  }
}


