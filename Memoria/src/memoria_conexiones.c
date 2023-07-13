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
#include <string.h>

//int idProceso = 0;

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

  list_destroy(listaSegmentos);
  liberarPaquete(paquete);
  free(posicion);
}

/* Chequear tipos
void eliminarSegmentoDeBitArray(segmento) {
  bitMapSegmento -> sobre lo que operamos -> es variable global
  base -> la posicion en la memoria -> se obtiene del segmento
  limite -> la cantidad de 1 -> se obtiene del segmento

  for (int i = base; i < base + limite; i++) {
    el i representa la posicion en el array de bits
    bitarray_clean_bit(t_bitarray*, off_t bit_index);
  }

}
*/

/* Chequea tipos
contexto eliminarSegmento(contexto, segmento, posicion) {
  [] -> obtenes la lista del contexto

  eliminarSegmentoDeBitArray(segmento); ->   Modificar el array de bits (sacar 1)
  [Liberar el segmento en especifico de la tablaDeSegmentos global en memoria] <- Todavia no esta implementado (lo hace lucio)
  Sacar segmento del contexto -> usas posicion + una funcion de list.h para sacar cosas de listas;
  retorn contextoModificado;
}
*/

Segmento* generarSegmentoAuxiliar(Segmento* segmentoNuevo) {
  Segmento* segmentoAuxiliar = malloc(sizeof(Segmento));
  segmentoAuxiliar->base = segmentoNuevo->base;
  segmentoAuxiliar->id = segmentoNuevo->id;
  segmentoAuxiliar->limite = segmentoNuevo->limite;
  return segmentoAuxiliar;
}

void procesarOperacion(op_code codigoOperacion, int socketCliente) {
  t_buffer* buffer;
  contextoEjecucion* contexto;

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
      //tablaDeSegmento* nuevaTabla = malloc(sizeof(tablaDeSegmento));
      //nuevaTabla->id = idProceso++;
      //nuevaTabla->segmentos_proceso = list_create();
      //list_add(tablaDeSegmentosPorProceso, nuevaTabla);
      buffer = obtenerBuffer(socketCliente);
      enviarSegmentoCero(socketCliente);
      liberarBuffer(buffer);
      break;
    case CREATE_SEGMENT:
      puts("crear segmento");
      contexto = recibirContexto(socketCliente);
      //int idProceso = recibirEntero(socketCliente);
      int idSegmento = recibirEntero(socketCliente);
      int tamanioSegmento = recibirEntero(socketCliente);
      op_code respuestaMemoria;

      if (puedoGuardar(tamanioSegmento)) {
        printf("Create segment %d %d\n", idSegmento, tamanioSegmento);
        Segmento* segmentoNuevo = crearSegmento(idSegmento, tamanioSegmento);
        Segmento* auxiliar = generarSegmentoAuxiliar(segmentoNuevo);
        t_list* listaSegmentos = contexto->tablaSegmentos;
        //tablaDeSegmento* nuevaTabla = list_get(tablaDeSegmentosPorProceso, idProceso);
        //list_add(nuevaTabla->segmentos_proceso, segmentoNuevo);
        list_add(listaSegmentos, segmentoNuevo);
        printf("base nuevo segmento %d\n", segmentoNuevo->base);
        respuestaMemoria = Pcb;
      } else {
        puts("Out of Memory");
        respuestaMemoria = OUT_OF_MEMORY;
      }
      printf("cantidad de segmentos %d", contexto->tablaSegmentos->elements_count);
      printf("Envia Respuesta a Kernel codigo %d\n", respuestaMemoria);
      enviarContexto(contexto, socketCliente, respuestaMemoria);
      liberarContexto(contexto);
      break;
    case DELETE_SEGMENT:
      puts("Llego delete Segment");

      contexto = recibirContexto(socketCliente);
      int idSeg = recibirEntero(socketCliente);
      printf("Segmento %d\n", idSeg);
      // Buscar sobre la lista de segmentos el la posicion del id

      // Fijate el tipo de retorno del list_get() [Es un puntero, asi que trabajas con punteros]
      // Obtener el segmento apartir del id <- Funcion commons list.h [list_get(lista, posicion)] operas con la lista de segmentos del contexto
      // EliminarSegmento(....);
      // Se devuelve el contexto eliminado al kernel
      liberarContexto(contexto);
      break;
    case SUCCESS:
      puts("Termino proceso Exitosamente");
      buffer = obtenerBuffer(socketCliente);
      liberarBuffer(buffer);
      liberarRecursos();
      exit(-1);
      break;
    case OUT_OF_MEMORY:
      puts("Termino proceso OUT OF MEMORY");
      buffer = obtenerBuffer(socketCliente);
      liberarBuffer(buffer);
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
