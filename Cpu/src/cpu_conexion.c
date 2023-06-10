#include <commons/log.h>
#include <conexiones.h>
#include <cpu_conexion.h>
#include <estructuras.h>
#include <pthread.h>
#include <recursos.h>
#include <serializacion/contexto.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

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

op_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoOperacion;
  char* identificador = instruccion->strings[0];

  if (strcmp("YIELD", identificador) == 0) {
    puts("Estoy ejecutando YIELD");
    *continuarEjecutando = 0;
    codigoOperacion = YIELD;
  } else if (strcmp("EXIT", identificador) == 0) {
    puts("Estoy ejecutando EXIT");
    *continuarEjecutando = 0;
    codigoOperacion = EXIT;
  }
  return codigoOperacion;
}

op_code ejecutarInstruccion(contextoEjecucion* contexto, t_instruccion* instruccion, int* continuarEjecutando) {
  op_code codigoPaquete;
  switch (instruccion->cantidadParametros) {
    case 0:
      codigoPaquete = ejecutarCeroParametros(contexto, instruccion, continuarEjecutando);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
  }
  return codigoPaquete;
}

void ejecutarListaInstrucciones(contextoEjecucion* contexto) {
  t_list* listaInstrucciones = contexto->instrucciones;
  op_code codigoPaquete;
  int* continuarEjecutando = malloc(sizeof(int));
  *continuarEjecutando = 1;

  while(*continuarEjecutando) {
      t_instruccion* instruccion = list_get(listaInstrucciones, contexto->programCounter);
      contexto->programCounter++;
      codigoPaquete = ejecutarInstruccion(contexto, instruccion, continuarEjecutando);
  }
  puts("Estoy enviando al kernel instruccion");
  enviarContexto(contexto, recursosCpu->conexiones->socketKernel, codigoPaquete);
  //liberarContexto(contexto);
}

void ejecutarProceso() {
  int socketKernel = recursosCpu->conexiones->socketKernel;
  while(1) {
    op_code codigoOperacion = obtenerCodigoOperacion(socketKernel);
    switch(codigoOperacion) {
      case Pcb:
        contextoEjecucion* contexto = recibirContexto(socketKernel);
        mostrarContexto(contexto);
        ejecutarListaInstrucciones(contexto);
        break;
      default:
        puts("Como carajo llegue aca");
    }
  }
}

void manejarConexionConKernel() {
  pthread_t hiloKernel;
  pthread_create(&hiloKernel, NULL, (void*)ejecutarProceso, NULL);
  pthread_detach(hiloKernel);
}

void montarServidor() {
  contextoEjecucion* contexto;
  t_log* logger = recursosCpu->logger;
  int socketServidor = recursosCpu->conexiones->socketCpu;

  log_info(logger, "Servidor Cpu se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    manejarConexionConKernel(socketCliente);
    recursosCpu->conexiones->socketKernel = socketCliente;
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
