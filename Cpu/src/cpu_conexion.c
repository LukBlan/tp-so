#include <commons/string.h>
#include <conexiones.h>
#include <cpu_conexion.h>
#include <recursos.h>
#include <estructuras.h>
#include <netdb.h>

void cargarConexiones() {
  conectarConMemoria();
  generarServidorCpu();
}

void conectarConMemoria() {
  t_configuracion* config = recursosCpu->configuracion;
  t_log* logger = recursosCpu->logger;

  log_info(logger, "Conectando con el Servidor Memoria...");
  int socketMemoria = crearConexionServidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);

  if (socketMemoria < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Memoria no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosCpu->conexiones->socketMemoria = socketMemoria;
}

void generarServidorCpu() {
  t_configuracion* config = recursosCpu->configuracion;
  t_log* logger = recursosCpu->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor Cpu...");
  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor Cpu.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Socket del Servidor Cpu Creado Exitosamente.");
  recursosCpu->conexiones->socketCpu = socketServidor;
}

void montarServidor() {
  t_log* logger = recursosCpu->logger;
  int socketServidor = recursosCpu->conexiones->socketCpu;

  log_info(logger, "Servidor Cpu se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    close(socketCliente);
  }
}

void realizarHandshakeMemoria() {
  t_log* logger = recursosCpu->logger;
  int socketMemoria = recursosCpu->conexiones->socketMemoria;

  log_info(logger, "Realizando Handshake con Memoria");
  if (realizarHandshake(logger, socketMemoria) < 0) {
    liberarRecursos();
    exit(-1);
  }
}
