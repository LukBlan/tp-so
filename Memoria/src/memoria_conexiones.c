#include <stdlib.h>
#include <conexiones.h>
#include <memoria_conexiones.h>
#include <recursos.h>
#include <pthread.h>
#include <netdb.h>
#include <estructuras.h>
#include <serializacion/paquete.h>
#include <serializacion/buffer.h>
#include <serializacion/contexto.h>
#include <segmentacion.h>

void cargarConexiones() {
  t_configuracion* config = recursosMemoria->configuracion;
  t_log* logger = recursosMemoria->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor Memoria...");

  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor Memoria.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Socket del Servidor Memoria Creado Exitosamente.");
  recursosMemoria->conexiones->socketMemoria = socketServidor;
}

void montarServidor() {
  int socketServidor = recursosMemoria->conexiones->socketMemoria;
  t_log* logger = recursosMemoria->logger;

  log_info(logger, "Servidor Memoria se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    manejarConexion(socketCliente);
  }
}

void enviarSegmentoCero(int socketCliente) {
  int* posicion = malloc(sizeof(int));
  *posicion = 0;
  t_list* listaSegmentos = list_create();
  list_add(listaSegmentos, segmentoCero);
  int tamanioListaSegmentos = tamanioBytesSegmentos(listaSegmentos);

  t_buffer* buffer = generarBuffer(tamanioListaSegmentos);
  serializarSegmentos(buffer, listaSegmentos, posicion);
  t_paquete* paquete = crearPaquete(buffer, Pcb);
  enviar_paquete(paquete, socketCliente);
}

void procesarOperacion(op_code codigoOperacion, int socketCliente) {
  printf("Estoy procesando conexion %d\n", codigoOperacion);
  switch (codigoOperacion) {
    case HANDSHAKE:
      puts("Entre handshake");
      int valorRecibido = 0;
      log_info(recursosMemoria->logger, "Recibido Pedido de Handshake, Respondiendo");
      recv(socketCliente, &valorRecibido, sizeof(int), 0);
      send(socketCliente, &valorRecibido, sizeof(int), 0);
      break;
    case Pcb:
      puts("Entre pcb");
      t_buffer* buffer = obtenerBuffer(socketCliente);
      enviarSegmentoCero(socketCliente);
      break;
    case CREATE_SEGMENT:
      puts("crear segmento");
      contextoEjecucion* contexto = recibirContexto(socketCliente);
      char* idSegmento = recibirString(socketCliente);
      char* tamanioSegmento = recibirString(socketCliente);
      printf("Create segment %s %s\n", idSegmento, tamanioSegmento);
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
