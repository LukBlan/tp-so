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
  printf("El proceso %d tiene %d segmentos\n", idProceso, nuevaTabla->segmentos_proceso->elements_count);
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

  if (posicion == -1) {
    log_error(recursosMemoria->logger, "El segmento a elimnar NO esta en el contexto");
  }
  return posicion;
}

void eliminarSegmentoDeTabla(int idProceso, int posicionEnContexto) {
  int posicionEnTabla = obtenerPosicionProcesoEnTabla(idProceso);
  tablaDeSegmento* tablaSegmento = list_get(tablaDeSegmentosPorProceso, posicionEnTabla);
  t_list* listaSegmentos = tablaSegmento->segmentos_proceso;
  printf("Cantidad segmentos %d en proceso %d\n", listaSegmentos->elements_count, idProceso);
  Segmento* segmento = list_remove(listaSegmentos, posicionEnContexto - 1);
  printf("Cantidad segmentos %d en proceso %d\n", listaSegmentos->elements_count, idProceso);
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
  t_buffer* buffer;
  contextoEjecucion* contexto;
  int retardoMemoria = recursosMemoria->configuracion->RETARDO_MEMORIA * 1000;

  printf("Estoy procesando conexion %d\n", codigoOperacion);
  switch (codigoOperacion) {
    case HANDSHAKE:
      puts("------------- Entre handshake -------------");
      int valorRecibido = 0;
      log_info(recursosMemoria->logger, "Recibido Pedido de Handshake, Respondiendo");
      recv(socketCliente, &valorRecibido, sizeof(int), 0);
      send(socketCliente, &valorRecibido, sizeof(int), 0);
      break;

    case Pcb:
      puts("------------ Entre pcb ----------------");
      tablaDeSegmento* nuevaTabla = malloc(sizeof(tablaDeSegmento));
      nuevaTabla->id = idProceso++;
      nuevaTabla->segmentos_proceso = list_create();
      list_add(tablaDeSegmentosPorProceso, nuevaTabla);
      printf("cantidad de procesos %d\n", tablaDeSegmentosPorProceso->elements_count);
      buffer = obtenerBuffer(socketCliente);
      enviarSegmentoCero(socketCliente);
      liberarBuffer(buffer);
      break;

    case CREATE_SEGMENT:
      puts("----------- Entre Create_Segmento ---------------");
      contexto = recibirContexto(socketCliente);
      int idProceso = recibirEntero(socketCliente);
      int idSegmento = recibirEntero(socketCliente);
      int tamanioSegmento = recibirEntero(socketCliente);
      respuestaMemoria = Pcb; // Se usa cuando se compacta

      if (puedoGuardar(tamanioSegmento)) {
        printf("Create segment %d %d\n", idSegmento, tamanioSegmento);
        usleep(retardoMemoria);
        Segmento* segmentoNuevo = crearSegmento(idSegmento, tamanioSegmento);
        agregarSegmentoATabla(segmentoNuevo, idProceso);
        agregarSegmentoAContexto(contexto, segmentoNuevo);
        printf("Segmento elegido base: %d\n", segmentoNuevo->base);
      } else {
        puts("Out of Memory");
        respuestaMemoria = OUT_OF_MEMORY;
      }

      if (respuestaMemoria == COMPACTACION) {
        puts("EEEEEEEEEEnviado Tabla");
        enviarTablaDeSegmentos(tablaDeSegmentosPorProceso, socketCliente, COMPACTACION);
      } else {
        enviarContexto(contexto, socketCliente, respuestaMemoria);
      }

      liberarContexto(contexto);
      break;

    case DELETE_SEGMENT:
      puts("--------------- Entre Delete_Segment -------------");
      contexto = recibirContexto(socketCliente);
      int idPro = recibirEntero(socketCliente); // id de proceso para elimnar de la tabla global
      int idSeg = recibirEntero(socketCliente);
      printf("Proceso: %d Segmento %d\n", idPro, idSeg);
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
      puts("------------- Entre Success ----------------");
      buffer = obtenerBuffer(socketCliente);
      int idProcesSucess = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesSucess);
      int cantidadDeCeros = contarCantidadDe(1024, 0);
      printf("El proceso termino con una cantidad de 0 = a %d\n", cantidadDeCeros);
      liberarBuffer(buffer);
      break;

    case OUT_OF_MEMORY:
      puts("------------- Entre OUT OF MEMORY -------------");
      buffer = obtenerBuffer(socketCliente);
      int idProcesOut = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesOut);

      liberarBuffer(buffer);
      break;
    case INVALID_RESOURCE:
      puts("------------- Entre INVALID_RESOURCE -------------");
      buffer = obtenerBuffer(socketCliente);
      int idProcesoInvalido = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesoInvalido);

      liberarBuffer(buffer);
      break;
    case SEGMENTATION_FAULT:
      puts("------------- Entre SEGMENTATION_FAULTSEGMENTATION_FAULT -------------");
      buffer = obtenerBuffer(socketCliente);
      int idProcesoFault = recibirEntero(socketCliente);
      eliminarSegmentosDeProceso(idProcesoFault);

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
