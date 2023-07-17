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
    manejarConexion(socketCliente);
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

void procesarOperacionRecibida(int socketCliente) {
  int conexionActiva = 1;
  while(conexionActiva) {
    int codigoOperacion;
    if (recv(socketCliente, &codigoOperacion, sizeof(int), 0) <= 0) {
      conexionActiva = 0;
      close(socketCliente);
    } else {
      procesarOperacion(codigoOperacion, socketCliente);
    }
  }
}
void procesarOperacion(op_code codigoOperacion, int socketCliente) {
  t_buffer* buffer;
  contextoEjecucion* contexto;

  printf("Estoy procesando conexion %d\n", codigoOperacion);
  switch (codigoOperacion) {
    case F_OPEN:
      puts ("Llego F_open");
      contexto = recibirContexto(socketCliente);
      char* nomArchivo = recibirString(socketCliente);
      puts("aca llego");
      contexto = fcreate(nomArchivo,contexto);
      puts("aca llego");
      enviarContexto(contexto,socketCliente,SUCCESS);
      liberarContexto(contexto);
    break;
    default:
      puts("Cerre una conexion");
      /*
      close(socketCliente);
      */
      break;
  }
}

void manejarConexion(int socketCliente) {
  pthread_t hilo;
  pthread_create(&hilo, NULL, (void*)procesarOperacionRecibida, (void*)socketCliente);
  pthread_detach(hilo);
}


