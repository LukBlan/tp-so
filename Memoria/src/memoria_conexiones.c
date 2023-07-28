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

int idProceso = 0;

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

Segmento* generarSegmentoAuxiliar(Segmento* segmentoNuevo) {
  Segmento* segmentoAuxiliar = malloc(sizeof(Segmento));
  segmentoAuxiliar->base = segmentoNuevo->base;
  segmentoAuxiliar->id = segmentoNuevo->id;
  segmentoAuxiliar->limite = segmentoNuevo->limite;
  return segmentoAuxiliar;
}

void agregarSegmentoATabla(Segmento* segmentoNuevo, int idProceso) {
  int posicionEnTabla = obtenerPosicionProcesoEnTabla(idProceso);
  tablaDeSegmento* nuevaTabla = list_get(tablaDeSegmentosPorProceso, posicionEnTabla);
  list_add(nuevaTabla->segmentos_proceso, segmentoNuevo);
}

void agregarSegmentoAContexto(contextoEjecucion* contexto, Segmento* segmentoNuevo) {
  Segmento* auxiliar = generarSegmentoAuxiliar(segmentoNuevo);
  t_list* listaSegmentos = contexto->tablaSegmentos;
  list_add(listaSegmentos, auxiliar);
}

int obtenerPosicionSegmento(contextoEjecucion* contexto, int idSeg) {
  t_list* listaSegmentos = contexto->tablaSegmentos;
  int cantidadDeSegmentos = listaSegmentos->elements_count;
  int posicion = -1;

  for (int i = 0; i < cantidadDeSegmentos; i++) {
    Segmento* segmento = list_get(listaSegmentos, i);
    if (segmento->id == idSeg) {
      posicion = i;
    }
  }
  return posicion;
}

void eliminarSegmentoDeTabla(int idProceso, int posicionEnContexto) {
  int posicionEnTabla = obtenerPosicionProcesoEnTabla(idProceso);
  tablaDeSegmento* tablaSegmento = list_get(tablaDeSegmentosPorProceso, posicionEnTabla);
  t_list* listaSegmentos = tablaSegmento->segmentos_proceso;
  Segmento* segmento = list_remove(listaSegmentos, posicionEnContexto - 1);
  log_info(
    recursosMemoria->logger,
    "PID: <%d> - Eliminar Segmento: <%d> - Base: <%d> - TAMAÑO: <%d>",
    idProceso, segmento->id, segmento->base, segmento->limite
  );
  free(segmento);
}

void eliminarSegmentoDeContexto(contextoEjecucion* contexto, int posicionEnContexto) {
  t_list* listaSegmentos = contexto->tablaSegmentos;
  Segmento* segmento = list_remove(listaSegmentos, posicionEnContexto);
  free(segmento);
}

void eliminarSegmentoDelArray(Segmento* segmento) {
  int tamanioSegmento = segmento->limite;
  int baseSegmento = segmento->base;
  for (int i = baseSegmento * 8; i < (baseSegmento + tamanioSegmento) * 8; i++) {
    bitarray_clean_bit(bitMapSegmento, i);
  }
}

void elimnarSegmentoDeBitArray(contextoEjecucion* contexto, int posicionEnContexto) {
  Segmento* segmentoAEliminar = list_get(contexto->tablaSegmentos, posicionEnContexto);
  eliminarSegmentoDelArray(segmentoAEliminar);
}

int obtenerPosicionProcesoEnTabla(int idProceso) {
  int cantidadDeProcesosEnTabla = tablaDeSegmentosPorProceso->elements_count;
  int posicion = -1;

  for (int i = 0; i < cantidadDeProcesosEnTabla; i++) {
    tablaDeSegmento* segmentosEnProceso = list_get(tablaDeSegmentosPorProceso, i);
    if (segmentosEnProceso->id == idProceso) {
      posicion = i;
    }
  }
  return posicion;
}

void eliminarSegmentosDeProceso(int procesoId) {
  int posicionEnTabla = obtenerPosicionProcesoEnTabla(procesoId);
  tablaDeSegmento* segmentosEnProceso = list_remove(tablaDeSegmentosPorProceso, posicionEnTabla);
  t_list* listSegmentos = segmentosEnProceso->segmentos_proceso;
  int cantidadSegmentos = listSegmentos->elements_count;
  for (int i = 0; i < cantidadSegmentos; i++) {
    Segmento* segmento = list_get(listSegmentos, i);
    eliminarSegmentoDelArray(segmento);
    free(segmento);
  }
  list_destroy(segmentosEnProceso->segmentos_proceso);
}

void procesarOperacion(op_code codigoOperacion, int socketCliente) {
  t_log* logger = recursosMemoria->logger;
  t_buffer* buffer;
  contextoEjecucion* contexto;
  int retardoMemoria = recursosMemoria->configuracion->RETARDO_MEMORIA * 1000;

  switch (codigoOperacion) {
    case HANDSHAKE:
      int valorRecibido = 0;
      log_info(recursosMemoria->logger, "Recibido Pedido de Handshake, Respondiendo");
      recv(socketCliente, &valorRecibido, sizeof(int), 0);
      send(socketCliente, &valorRecibido, sizeof(int), 0);
      break;

    case Pcb:
      tablaDeSegmento* nuevaTabla = malloc(sizeof(tablaDeSegmento));
      nuevaTabla->id = idProceso++;
      log_info(logger, "Creación de Proceso PID: <%d>", nuevaTabla->id);
      nuevaTabla->segmentos_proceso = list_create();
      list_add(tablaDeSegmentosPorProceso, nuevaTabla);
      buffer = obtenerBuffer(socketCliente);
      enviarSegmentoCero(socketCliente);
      liberarBuffer(buffer);
      break;

    case CREATE_SEGMENT:
      contexto = recibirContexto(socketCliente);
      int idProceso = recibirEntero(socketCliente);
      int idSegmento = recibirEntero(socketCliente);
      int tamanioSegmento = recibirEntero(socketCliente);
      respuestaMemoria = Pcb; // Se usa cuando se compacta
      clienteActual = socketCliente;
      if (puedoGuardar(tamanioSegmento)) {
        usleep(retardoMemoria);
        Segmento* segmentoNuevo = crearSegmento(idSegmento, tamanioSegmento);
        log_info(logger,
                "PID: <%d> - Crear Segmento: <%d> - Base: <%d> - TAMAÑO: <%d>",
                idProceso, segmentoNuevo->id, segmentoNuevo->base, segmentoNuevo->limite
        );
        agregarSegmentoATabla(segmentoNuevo, idProceso);
        agregarSegmentoAContexto(contexto, segmentoNuevo);
      } else {
        respuestaMemoria = OUT_OF_MEMORY;
      }

      if (respuestaMemoria == COMPACTACION) {
        enviarTablaDeSegmentos(tablaDeSegmentosPorProceso, socketCliente, COMPACTACION);
      } else {
        enviarContexto(contexto, socketCliente, respuestaMemoria);
      }

      liberarContexto(contexto);
      break;

    case DELETE_SEGMENT:
      contexto = recibirContexto(socketCliente);
      int idPro = recibirEntero(socketCliente); // id de proceso para elimnar de la tabla global
      int idSeg = recibirEntero(socketCliente);
      int posicionEnContexto = obtenerPosicionSegmento(contexto, idSeg);
      elimnarSegmentoDeBitArray(contexto, posicionEnContexto);
      eliminarSegmentoDeTabla(idPro, posicionEnContexto);
      usleep(retardoMemoria);
      eliminarSegmentoDeContexto(contexto, posicionEnContexto);
      enviarContexto(contexto, socketCliente, DELETE_SEGMENT);
      liberarContexto(contexto);
      break;

    case MOV_IN:
      puts("--------------- Entre MOV_IN -------------");
      contexto = recibirContexto(socketCliente);
      int posicion = recibirEntero(socketCliente); // id de proceso para elimnar de la tabla global
      int tamanio = recibirEntero(socketCliente);

      char* cosaAEnviar = malloc(tamanio + 1);
      usleep(retardoMemoria);
      memcpy(cosaAEnviar, memoriaPrincipal + posicion, tamanio);
      cosaAEnviar[tamanio] = '\0';

      enviarString(cosaAEnviar, socketCliente);
      liberarContexto(contexto);
      break;

      case MOV_OUT:
      puts("--------------- Entre MOV_OUT -------------");
      contexto = recibirContexto(socketCliente);
      int posicionAMovear = recibirEntero(socketCliente); // id de proceso para elimnar de la tabla global
      char* cosaAEscribir = recibirString(socketCliente);
      usleep(retardoMemoria);
      memcpy(memoriaPrincipal+posicionAMovear, cosaAEscribir, strlen(cosaAEscribir));
      enviarContexto(contexto, socketCliente, SUCCESS);
      liberarContexto(contexto);
      break;

      case F_WRITE_MEMORIA:
      puts("Llego F_WRITE");
      contexto = recibirContexto(socketCliente);
      int direccionAEscribir = recibirEntero(socketCliente);
      int tamanioAEscribir = recibirEntero(socketCliente);
      char* cosaEscrita = malloc(tamanioAEscribir + 1);
      usleep(retardoMemoria);
      memcpy(cosaEscrita, memoriaPrincipal+direccionAEscribir, tamanioAEscribir);

      cosaEscrita[tamanioAEscribir] = '\0';
      enviarContexto(contexto,socketCliente,SUCCESS_WRITE_MEMORY);
      enviarString(cosaEscrita, socketCliente);
      break;

      case F_READ:
      puts("Llego F_READ");
      contexto = recibirContexto(socketCliente);
      char* cosaParaEscribir = recibirString(socketCliente);
      int posicionAEscribir = recibirEntero(socketCliente);
      usleep(retardoMemoria);
      memcpy(memoriaPrincipal+posicionAEscribir, cosaParaEscribir, strlen(cosaParaEscribir));
      enviarContexto(contexto,socketCliente,SUCCESS_READ_MEMORY);
      break;

    case SUCCESS:
      buffer = obtenerBuffer(socketCliente);
      int idProcesSucess = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesSucess);
      int cantidadDeCeros = contarCantidadDe(1024, 0);
      log_info(logger, "Eliminación de Proceso PID: <%d>", idProcesSucess);
      liberarBuffer(buffer);
      break;

    case OUT_OF_MEMORY:
      buffer = obtenerBuffer(socketCliente);
      int idProcesOut = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesOut);
      log_info(logger, "Eliminación de Proceso PID: <%d>", idProcesOut);
      liberarBuffer(buffer);
      break;
    case INVALID_RESOURCE:
      buffer = obtenerBuffer(socketCliente);
      int idProcesoInvalido = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesoInvalido);
      log_info(logger, "Eliminación de Proceso PID: <%d>", idProcesoInvalido);
      liberarBuffer(buffer);
      break;
    case SEGMENTATION_FAULT:
      buffer = obtenerBuffer(socketCliente);
      int idProcesoFault = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesoFault);
      log_info(logger, "Eliminación de Proceso PID: <%d>", idProcesoFault);
      liberarBuffer(buffer);
      break;
    default:
      puts("------------- Entre Default -------------");
      /*
      close(socketCliente);
      */
      break;
  }
}

void manejarConexion(int socketCliente) {
  pthread_t hilo;
  pthread_create(&hilo, NULL, (void*) procesarOperacionRecibida, (void*) socketCliente);
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
