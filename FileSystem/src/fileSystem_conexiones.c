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
  int socketMemoria = recursosFileSystem->conexiones->socketMemoria;

  switch (codigoOperacion) {
    case F_OPEN:
      contexto = recibirContexto(socketCliente);

      char* nomArchivo = recibirString(socketCliente);
      log_info(recursosFileSystem->logger, "Abrir Archivo - Nombre: %s", nomArchivo);
      contexto = fcreate(nomArchivo, contexto);
      enviarContexto(contexto,socketCliente, SUCCESS_OPEN);
      liberarContexto(contexto);
    break;

    case F_TRUNCATE:
      contexto = recibirContexto(socketCliente);
      char* nombreArchivo = recibirString(socketCliente);
      int tamanio = recibirEntero(socketCliente);
      contexto = ftruncar(nombreArchivo, contexto,tamanio);

      enviarContexto(contexto, socketCliente, SUCCESS_TRUNCATE);
      liberarContexto(contexto);
      break;

      case F_WRITE:
      contexto=recibirContexto(socketCliente);
      char* nombreDeArchivo = recibirString(socketCliente);
      int direccionAEscribir = recibirEntero(socketCliente);
      int tamanioWrite = recibirEntero(socketCliente);
      int posicionWrite = recibirEntero(socketCliente);
      log_info(recursosFileSystem->logger, "Escribir de Archivo - Nombre: %s -Puntero: %d - Memoria: %d- Tamaño: %d ", nombreDeArchivo,posicionWrite,direccionAEscribir,tamanioWrite);
      enviarContexto(contexto,socketMemoria,F_WRITE_MEMORIA);
      enviarEntero(direccionAEscribir,socketMemoria);
      enviarEntero(tamanioWrite,socketMemoria);
      op_code respuestaMemoriaEscritura = obtenerCodigoOperacion(socketMemoria);

      switch(respuestaMemoriaEscritura) {
        case SUCCESS_WRITE_MEMORY:
          contexto = recibirContexto(socketMemoria);
          char* datosParaEscribir = recibirString(socketMemoria);

          fEscritura(nombreDeArchivo,posicionWrite,datosParaEscribir,tamanioWrite);
          enviarContexto(contexto,socketCliente,SUCCESS_WRITE);
          liberarContexto(contexto);
          break;

          default:
            break;
      }
      break;
      
      case F_READ:
        contexto = recibirContexto(socketCliente);
        char* nombreArchivoLeer = recibirString(socketCliente);
        int tamanioARead = recibirEntero(socketCliente);
        int posicionARead = recibirEntero(socketCliente);
        int posicionArch = recibirEntero(socketCliente);
        log_info(recursosFileSystem->logger, "Lectura de Archivo - Nombre: %s -Puntero: %d - Memoria: %d- Tamaño: %d ", nombreArchivoLeer,posicionArch,posicionARead,tamanioARead);
        char* datoLeido = fLectura(nombreArchivoLeer,posicionArch,tamanioARead);

        enviarContexto(contexto,socketMemoria,F_READ);
        enviarString(datoLeido,socketMemoria);
        enviarEntero(posicionARead,socketMemoria);
        op_code respuestaMemoria = obtenerCodigoOperacion(socketMemoria);

        switch(respuestaMemoria) {
          case SUCCESS_READ_MEMORY:
            contexto = recibirContexto(socketMemoria);
            enviarContexto(contexto,socketCliente,SUCCESS_READ);
            liberarContexto(contexto);
            break;

          default:
            break;
        }
      break;

    default:
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


